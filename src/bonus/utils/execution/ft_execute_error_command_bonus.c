/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_error_command.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42madrid>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2024/12/19 11:30:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static void	ft_setup_error_child_process(t_cmd *cmd_list, t_cmd *head)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (cmd_list->infd != STDIN_FILENO)
		close(cmd_list->infd);
	if (cmd_list->outfd != STDOUT_FILENO)
		close(cmd_list->outfd);
	ft_close_unused_fds(cmd_list, head);
	exit(1);
}

static void	ft_handle_error_parent_process(t_cmd *cmd_list, pid_t pid,
		pid_t *pids)
{
	pids[cmd_list->index] = pid;
	if (cmd_list->infd != STDIN_FILENO)
		close(cmd_list->infd);
	if (cmd_list->outfd != STDOUT_FILENO)
		close(cmd_list->outfd);
}

int	ft_execute_error_command(t_cmd *cmd_list, t_cmd *head, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		ft_setup_error_child_process(cmd_list, head);
	else if (pid > 0)
	{
		ft_handle_error_parent_process(cmd_list, pid, pids);
		return (0);
	}
	else
	{
		perror("fork");
		return (-1);
	}
	return (0);
}
