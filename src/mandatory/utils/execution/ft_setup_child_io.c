/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setup_child_io.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:29:06 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:38:47 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Sets up the input and output file descriptors for a child process,
 *          duplicating them to standard input/output and closing
 * 			unused file descriptors.
 *
 * SPANISH: Configura los descriptores de archivo de entrada y salida
 * 			para un proceso hijo, duplicándolos a la entrada/salida
 * 			estándar y cerrando los descriptores de archivo no utilizados.
 *
 * @param current     Pointer to the current command structure. /
 *                    Puntero a la estructura del comando actual.
 *
 * @param cmd_list    Pointer to the head of the command list. /
 *                    Puntero al inicio de la lista de comandos.
 */
void	ft_setup_child_io(t_cmd *current, t_cmd *cmd_list)
{
	t_cmd	*temp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (current->infd != STDIN_FILENO)
		dup2(current->infd, STDIN_FILENO);
	if (current->outfd != STDOUT_FILENO)
		dup2(current->outfd, STDOUT_FILENO);
	ft_close_unused_fds(current, cmd_list);
}
