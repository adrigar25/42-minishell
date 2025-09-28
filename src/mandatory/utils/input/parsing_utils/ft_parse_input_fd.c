/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_fd.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:31 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:53:01 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

/**
 * ENGLISH: Determines the appropriate file descriptor based on the
 *          redirection token and filename.
 *
 * SPANISH: Determina el descriptor de archivo apropiado según el
 *          token de redirección y el nombre del archivo.
 *
 * @param token     The redirection token ("<", ">", ">>", "<<"). /
 *                  El token de redirección ("<", ">", ">>", "<<").
 *
 * @param filename  The filename associated with the redirection. /
 *                  El nombre del archivo asociado con la redirección.
 *
 * @returns The file descriptor for the redirection, or -1 on error. /
 *          El descriptor de archivo para la redirección, o -1 en caso de error.
 */
int	ft_get_fd_from_token(char *token, char *filename)
{
	if (ft_strcmp(token, "<") == 0)
		return (ft_handle_infile(filename));
	else if (ft_strcmp(token, ">") == 0)
		return (ft_handle_outfile(filename, 0));
	else if (ft_strcmp(token, ">>") == 0)
		return (ft_handle_outfile(filename, 1));
	else
		return (ft_handle_heredoc(filename));
}

/**
 * ENGLISH: Assigns the given file descriptor to the appropriate field
 *          in the command structure based on the redirection token.
 *          Closes any previously assigned file descriptor if necessary.
 *
 * SPANISH: Asigna el descriptor de archivo dado al campo apropiado
 *          en la estructura del comando según el token de redirección.
 *          Cierra cualquier descriptor de archivo previamente asignado si
 *          es necesario.
 *
 * @param cmd     Pointer to the command structure to update. /
 *                Puntero a la estructura de comando a actualizar.
 *
 * @param token   The redirection token ("<", ">", ">>", "<<"). /
 *                El token de redirección ("<", ">", ">>", "<<").
 *
 * @param fd      The file descriptor to assign. /
 *                El descriptor de archivo a asignar.
 *
 * @returns void.
 */
void	ft_assign_fd(t_cmd *cmd, char *token, int fd)
{
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0)
	{
		if (cmd->infd != STDIN_FILENO)
			close(cmd->infd);
		cmd->infd = fd;
	}
	else
	{
		if (cmd->outfd != STDOUT_FILENO)
			close(cmd->outfd);
		cmd->outfd = fd;
	}
}
