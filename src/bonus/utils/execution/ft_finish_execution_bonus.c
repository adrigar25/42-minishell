/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_finish_execution_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:10:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 11:56:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static void	ft_free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		ft_free_matrix(cmd_list->argv);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	ft_finish_execution(pid_t *pids, int cmd_count, t_cmd *cmd_list,
		t_data *data)
{
	int	i;
	int	status;
	int	last_exit_status;
	int	executed_processes;

	last_exit_status = data->last_exit_status;
	executed_processes = 0;
	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_exit_status = 128 + WTERMSIG(status);
			executed_processes = 1;
		}
		i++;
	}
	if (executed_processes)
		data->last_exit_status = last_exit_status;
	free(pids);
	ft_free_cmd_list(cmd_list);
	cmd_list = NULL;
}
