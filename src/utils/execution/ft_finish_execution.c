/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_finish_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:10:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 20:06:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		ft_free_char_array(cmd_list->argv);
		free(cmd_list);
		cmd_list = tmp;
	}
}

void	ft_finish_execution(pid_t *pids, int cmd_count, t_cmd *cmd_list,
		t_data *data)
{
	int	i;
	int	status;

	i = cmd_count - 1;
	while (i >= 0)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				data->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->last_exit_status = 128 + WTERMSIG(status);
		}
		i--;
	}
	free(pids);
	ft_free_cmd_list(cmd_list);
	cmd_list = NULL;
}
