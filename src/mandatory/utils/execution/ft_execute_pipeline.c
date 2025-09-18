/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_pipeline.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:35:40 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 16:43:45 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
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
	pid_t	pid;

	if (!cmd_list || !data || !*data)
		return (-1);
	pids = malloc(sizeof(pid_t) * (*data)->cmd_count);
	if (!pids)
		return (-1);
	current = cmd_list;
	while (current)
	{
		if (ft_is_builtin(current) && current->infd == STDIN_FILENO
			&& current->outfd == STDOUT_FILENO && !current->next
			&& current->has_error == 0)
		{
			(*data)->last_exit_status = ft_exec_builtin(current, data);
			current = current->next;
			continue ;
		}
		pid = fork();
		if (pid == 0)
		{
			ft_setup_child_io(current, cmd_list);
			if (current->has_error)
				(*data)->last_exit_status = 1;
			else if (ft_is_builtin(current))
				(*data)->last_exit_status = ft_exec_builtin(current, data);
			else
				(*data)->last_exit_status = ft_exec_cmd(current);
			exit((*data)->last_exit_status);
		}
		else if (pid > 0)
		{
			pids[current->index] = pid;
			if (current->has_error == 1)
			{
				if (current->infd != STDIN_FILENO)
					close(current->infd);
				if (current->outfd != STDOUT_FILENO)
					close(current->outfd);
			}
		}
		else
		{
			perror("fork");
			ft_finish_execution(pids, cmd_list, *data);
			return ((*data)->last_exit_status);
		}
		current = current->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
