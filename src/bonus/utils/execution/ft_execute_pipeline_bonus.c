/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_pipeline.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 14:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 14:30:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <string.h>
#include <unistd.h>

static int	ft_is_builtin(t_cmd *current)
{
	char	*cmd;

	if (!current || !current->argv || !current->argv[0])
		return (0);
	cmd = current->argv[0];
	if (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 || strcmp(cmd,
			"pwd") == 0 || strcmp(cmd, "export") == 0 || strcmp(cmd,
			"unset") == 0 || strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	ft_execute_pipeline(t_cmd *cmd_list, t_data **data)
{
	t_cmd	*current;
	pid_t	*pids;
	int		exit_status;

	if (!cmd_list || !data || !*data)
		return (-1);
	pids = malloc(sizeof(pid_t) * (*data)->cmd_count);
	if (!pids)
		return (-1);
	exit_status = ft_execute_logical_chain(cmd_list, data, pids);
	free(pids);
	return (exit_status);
}

static int	ft_execute_single_cmd(t_cmd *cmd, t_data **data, pid_t *pids)
{
	pid_t	pid;
	t_cmd	*temp;
	int		status;

	if (cmd->has_error == 1)
		return (ft_execute_error_command(cmd, cmd, pids));
	if (ft_is_builtin(cmd))
		return (ft_handle_builtins(cmd, data, cmd, pids));
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		signal(SIGPIPE, SIG_DFL);
		if (cmd->infd != STDIN_FILENO)
			dup2(cmd->infd, STDIN_FILENO);
		if (cmd->outfd != STDOUT_FILENO)
			dup2(cmd->outfd, STDOUT_FILENO);
		temp = cmd;
		while (temp)
		{
			if (temp->infd != STDIN_FILENO && temp->infd != cmd->infd
				&& temp->infd != cmd->outfd)
				close(temp->infd);
			if (temp->outfd != STDOUT_FILENO && temp->outfd != cmd->infd
				&& temp->outfd != cmd->outfd)
				close(temp->outfd);
			temp = temp->next;
		}
		(*data)->last_exit_status = ft_exec_cmd(cmd);
		exit((*data)->last_exit_status);
	}
	else if (pid > 0)
	{
		pids[cmd->index] = pid;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
	else
	{
		perror("fork");
		return (1);
	}
}

static int	ft_execute_pipe_sequence(t_cmd *start, t_cmd **end, t_data **data,
		pid_t *pids)
{
	t_cmd	*current;
	pid_t	pid;
	t_cmd	*temp;
	int		status;
	int		last_exit_status;

	current = start;
	last_exit_status = 0;
	while (current && current->op == OP_PIPE)
	{
		if (ft_is_builtin(current))
		{
			last_exit_status = ft_handle_builtins(current, data, start, pids);
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				signal(SIGPIPE, SIG_DFL);
				if (current->infd != STDIN_FILENO)
					dup2(current->infd, STDIN_FILENO);
				if (current->outfd != STDOUT_FILENO)
					dup2(current->outfd, STDOUT_FILENO);
				temp = start;
				while (temp)
				{
					if (temp->infd != STDIN_FILENO
						&& temp->infd != current->infd
						&& temp->infd != current->outfd)
						close(temp->infd);
					if (temp->outfd != STDOUT_FILENO
						&& temp->outfd != current->infd
						&& temp->outfd != current->outfd)
						close(temp->outfd);
					temp = temp->next;
				}
				(*data)->last_exit_status = ft_exec_cmd(current);
				exit((*data)->last_exit_status);
			}
			else if (pid > 0)
				pids[current->index] = pid;
		}
		current = current->next;
	}
	if (current && current->op != OP_PIPE)
	{
		last_exit_status = ft_execute_single_cmd(current, data, pids);
		current = current->next;
	}
	temp = start;
	while (temp && temp != current)
	{
		if (temp->infd != STDIN_FILENO)
			close(temp->infd);
		if (temp->outfd != STDOUT_FILENO)
			close(temp->outfd);
		temp = temp->next;
	}
	*end = current;
	return (last_exit_status);
}

int	ft_execute_logical_chain(t_cmd *cmd_list, t_data **data, pid_t *pids)
{
	t_cmd	*current;
	t_cmd	*next;
	int		exit_status;
	int		should_execute;

	current = cmd_list;
	exit_status = 0;
	should_execute = 1;
	while (current)
	{
		if (should_execute)
		{
			if (current->next && current->next->op == OP_PIPE)
				exit_status = ft_execute_pipe_sequence(current, &next, data,
						pids);
			else
			{
				exit_status = ft_execute_single_cmd(current, data, pids);
				next = current->next;
			}
			(*data)->last_exit_status = exit_status;
		}
		else
			next = current->next;
		if (next)
		{
			if (next->op == OP_AND)
				should_execute = (exit_status == 0);
			else if (next->op == OP_OR)
				should_execute = (exit_status != 0);
		}
		current = next;
	}
	return (exit_status);
}
