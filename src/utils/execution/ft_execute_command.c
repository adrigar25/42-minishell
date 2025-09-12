/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42madrid>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2024/12/19 11:00:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_setup_child_process(t_cmd *cmd_list, t_cmd *head, t_data **data)
{
	int	builtin_result;
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (cmd_list->infd != STDIN_FILENO)
	{
		dup2(cmd_list->infd, STDIN_FILENO);
		close(cmd_list->infd);
	}
	if (cmd_list->outfd != STDOUT_FILENO)
	{
		dup2(cmd_list->outfd, STDOUT_FILENO);
		close(cmd_list->outfd);
	}
	ft_close_unused_fds(cmd_list, head);
	builtin_result = ft_handle_builtins(cmd_list, data);
	if (builtin_result != -1)
		exit(builtin_result);
	else
	{
		exit_status = ft_exec_cmd(cmd_list);
		if (exit_status == -1)
			exit(EXIT_FAILURE);
		exit(exit_status);
	}
}

static void	ft_handle_parent_process(t_cmd *cmd_list, pid_t pid, pid_t *pids)
{
	pids[cmd_list->index] = pid;
	if (cmd_list->infd != STDIN_FILENO)
		close(cmd_list->infd);
	if (cmd_list->outfd != STDOUT_FILENO)
		close(cmd_list->outfd);
}

int	ft_execute_command(t_cmd *cmd_list, t_cmd *head, pid_t *pids, t_data **data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_setup_child_process(cmd_list, head, data);
	}
	else if (pid > 0)
	{
		ft_handle_parent_process(cmd_list, pid, pids);
		return (0);
	}
	else
	{
		perror("fork");
		return (-1);
	}
	return (0);
}
