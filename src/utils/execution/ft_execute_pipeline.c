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
#include <unistd.h>

int	ft_execute_pipeline(t_cmd *cmd_list, pid_t *pids, t_data **data)
{
	t_cmd	*current;
	pid_t	pid;
	int		builtin_result;
	t_cmd	*temp;
	int		exit_status;

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
				builtin_result = ft_handle_builtins(current, data);
				if (builtin_result != -1)
				{
					(*data)->last_exit_status = builtin_result;
					exit(builtin_result);
				}
				exit_status = ft_exec_cmd(current);
				cmd_list->data->last_exit_status = exit_status;
				exit(exit_status);
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
