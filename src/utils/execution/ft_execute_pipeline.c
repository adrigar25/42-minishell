/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_pipeline.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
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

	current = cmd_list;
	while (current)
	{
		pid = fork();
		if (pid == 0)
		{
			// Proceso hijo
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGPIPE, SIG_DFL);

			// Configurar descriptores de archivo
			if (current->infd != STDIN_FILENO)
			{
				dup2(current->infd, STDIN_FILENO);
			}
			if (current->outfd != STDOUT_FILENO)
			{
				dup2(current->outfd, STDOUT_FILENO);
			}

			// Cerrar descriptores de otros comandos, pero no los del comando actual
			t_cmd *temp = cmd_list;
			while (temp)
			{
				if (temp != current)
				{
					if (temp->infd != STDIN_FILENO && temp->infd != current->infd
						&& temp->infd != current->outfd)
						close(temp->infd);
					if (temp->outfd != STDOUT_FILENO && temp->outfd != current->infd
						&& temp->outfd != current->outfd)
						close(temp->outfd);
				}
				temp = temp->next;
			}

			// Intentar ejecutar builtin primero
			builtin_result = ft_handle_builtins(current, data);
			if (builtin_result != -1)
				exit(builtin_result);

			// Ejecutar comando externo
			int exit_status = ft_exec_cmd(current);
			if (exit_status == -1)
				exit(EXIT_FAILURE);
			exit(exit_status);
		}
		else if (pid > 0)
		{
			// Proceso padre
			pids[current->index] = pid;
		}
		else
		{
			perror("fork");
			return (-1);
		}

		current = current->next;
	}

	// Cerrar todos los descriptores en el proceso padre DESPUÉS de crear todos los hijos
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
