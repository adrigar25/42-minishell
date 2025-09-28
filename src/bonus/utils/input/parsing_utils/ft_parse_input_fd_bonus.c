/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_fd_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:31 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:40:15 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

/**
 * ENGLISH: Opens a file descriptor based on the given token and filename.
 *           Handles input ('<'), output ('>'), append ('>>'),
 *           and heredoc ('<<') redirections.
 *
 * SPANISH: Abre un descriptor de archivo según el token y el nombre de
 *          archivo dados.
 *          Maneja redirecciones de entrada ('<'), salida ('>'),
 *          anexado ('>>') y heredoc ('<<').
 *
 * @param token    The redirection token ("<", ">", ">>", or "<<").
 *                 El token de redirección ("<", ">", ">>" o "<<").
 *
 * @param filename The name of the file to open or use for redirection.
 *                 El nombre del archivo a abrir o usar para la redirección.
 *
 * @returns The file descriptor on success, or -1 on failure.
 *          El descriptor de archivo en caso de éxito, o -1 en caso de error.
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
 *          en la estructura de comando según el token de redirección.
 *          Cierra cualquier descriptor de archivo previamente asignado
 *          si es necesario.
 *
 * @param cmd    Pointer to the command structure to update.
 *               Puntero a la estructura de comando a actualizar.
 *
 * @param token  The redirection token ("<", ">", ">>", or "<<").
 *               El token de redirección ("<", ">", ">>" o "<<").
 *
 * @param fd     The file descriptor to assign.
 *               El descriptor de archivo a asignar.
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
