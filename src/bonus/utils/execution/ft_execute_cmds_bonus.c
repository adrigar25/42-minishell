/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:32:13 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 16:23:02 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_execute_single_builtin(t_cmd *cmd, t_data **data)
{
	if (!ft_is_builtin(cmd))
		return (0);
	if (cmd->infd != STDIN_FILENO || cmd->outfd != STDOUT_FILENO)
		return (0);
	if (cmd->next || cmd->has_error)
		return (0);
	(*data)->last_exit_status = ft_exec_builtin(cmd, data);
	return (1);
}

static int	ft_fork_cmd(t_cmd *cmd, t_cmd *cmd_list, t_data **data, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_setup_child_io(cmd, cmd_list);
		if (cmd->has_error)
			(*data)->last_exit_status = 1;
		else if (ft_is_builtin(cmd))
			(*data)->last_exit_status = ft_exec_builtin(cmd, data);
		else
			(*data)->last_exit_status = ft_exec_bin(cmd);
		exit((*data)->last_exit_status);
	}
	if (pid > 0)
	{
		pids[cmd->index] = pid;
		if (cmd->infd != STDIN_FILENO)
			close(cmd->infd);
		if (cmd->outfd != STDOUT_FILENO)
			close(cmd->outfd);
		return (0);
	}
	return (perror("fork"), -1);
}

static void	ft_wait_last(int *status, t_data **data, pid_t *pids, int idx)
{
	if (pids[idx] <= 0)
		return ;
	waitpid(pids[idx], status, 0);
	if (WIFEXITED(*status))
		(*data)->last_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		(*data)->last_exit_status = 128 + WTERMSIG(*status);
	pids[idx] = 0;
}

static int	ft_exec_cmd(t_cmd *cmd, t_cmd *cmd_list, t_data **data, pid_t *pids)
{
	if (ft_should_execute(&cmd, *data))
		return (0);
	if (ft_execute_single_builtin(cmd, data))
		return (1);
	if (ft_fork_cmd(cmd, cmd_list, data, pids) == -1)
		return (-1);
	return (0);
}

int	ft_execute_cmds(t_cmd *cmd_list, t_data **data)
{
	t_cmd	*cmd;
	pid_t	*pids;
	int		status;

	if (!cmd_list || !data || !*data)
		return (-1);
	pids = malloc(sizeof(pid_t) * (*data)->cmd_count);
	if (!pids)
		return (-1);
	cmd = cmd_list;
	while (cmd)
	{
		if (ft_exec_cmd(cmd, cmd_list, data, pids) == -1)
			return (ft_finish_execution(pids, cmd_list, *data));
		if (!cmd->next || cmd->next->op != OP_PIPE)
			ft_wait_last(&status, data, pids, cmd->index);
		cmd = cmd->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
