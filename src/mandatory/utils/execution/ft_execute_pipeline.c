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

int	ft_execute_pipeline(t_cmd *cmd_list, pid_t *pids, t_data **data)
{
	t_cmd	*current;
	pid_t	pid;
	int		builtin_result;
	t_cmd	*temp;

	current = cmd_list;
	while (current)
	{
		if (current->has_error == 1)
		{
			if (current->outfd != STDOUT_FILENO)
				close(current->outfd);
			ft_execute_error_command(current, cmd_list, pids);
		}
		else
		{
			if (ft_is_builtin(current))
			{
				(*data)->last_exit_status = ft_handle_builtins(current, data,
						cmd_list, pids);
				current = current->next;
				continue ;
			}
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
				temp = cmd_list;
				while (temp)
				{
					if (temp != current)
					{
						if (temp->infd != STDIN_FILENO
							&& temp->infd != current->infd
							&& temp->infd != current->outfd)
							close(temp->infd);
						if (temp->outfd != STDOUT_FILENO
							&& temp->outfd != current->infd
							&& temp->outfd != current->outfd)
							close(temp->outfd);
					}
					else
					{
						if (temp->infd != STDIN_FILENO
							&& temp->infd != current->infd)
							close(temp->infd);
						if (temp->outfd != STDOUT_FILENO
							&& temp->outfd != current->outfd)
							close(temp->outfd);
					}
					temp = temp->next;
				}
				cmd_list->data->last_exit_status = ft_exec_cmd(current);
				exit(cmd_list->data->last_exit_status);
			}
			else if (pid > 0)
				pids[current->index] = pid;
			else
			{
				perror("fork");
				return (-1);
			}
		}
		current = current->next;
	}
	current = cmd_list;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		current = current->next;
	}
	return (0);
}
