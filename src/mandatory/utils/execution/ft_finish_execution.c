/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_finish_execution.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:10:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/16 16:49:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/*
** Frees the memory allocated for the command list.
*/
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

/**
 * ENGLISH: Closes file descriptors for all commands in the list except standard
 * 			input/output.
 *
 * SPANISH: Cierra los descriptores de archivo de todos los comandos en la lista
 * 			excepto la entrada/salida estándar.
 *
 * @param cmd_list   Pointer to the head of the command list. /
 *                   Puntero al inicio de la lista de comandos.
 */
static void	ft_close_fds(t_cmd *cmd_list)
{
	t_cmd	*current;

	current = cmd_list;
	while (current)
	{
		if (current->infd != STDIN_FILENO)
			close(current->infd);
		if (current->outfd != STDOUT_FILENO)
			close(current->outfd);
		current = current->next;
	}
}

static void	ft_handle_signaled(int status, int *last_exit_status)
{
	int	sig;

	sig = WTERMSIG(status);
	*last_exit_status = 128 + sig;
	if (sig == SIGQUIT)
	{
		if (WCOREDUMP(status))
			ft_putstr_fd("Quit (core dumped)\n", 2);
		else
			ft_putstr_fd("Quit\n", 2);
	}
}

static void	ft_wait_for_children(pid_t *pids, int cmd_count,
		int *last_exit_status)
{
	int	i;
	int	status;

	i = 0;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				*last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				ft_handle_signaled(status, last_exit_status);
		}
		i++;
	}
}

int	ft_finish_execution(pid_t *pids, t_cmd *cmd_list, t_data *data)
{
	int	last_exit_status;

	last_exit_status = data->last_exit_status;
	ft_close_fds(cmd_list);
	ft_wait_for_children(pids, data->cmd_count, &last_exit_status);
	data->last_exit_status = last_exit_status;
	free(pids);
	ft_free_cmd_list(cmd_list);
	cmd_list = NULL;
	if (data && data->argv)
	{
		ft_free_matrix(data->argv);
		data->argv = NULL;
	}
	return (last_exit_status);
}
