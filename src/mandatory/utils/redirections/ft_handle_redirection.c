/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redirection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:44:02 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/26 17:56:18 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Updates the command's input or output file descriptor based on the
 *          redirection type, closing any previously set descriptors.
 *
 * SPANISH: Actualiza el descriptor de archivo de entrada o salida del comando
 *          según el tipo de redirección, cerrando cualquier descriptor
 *          previamente establecido.
 *
 * @param cmd    Pointer to the command structure to update. /
 *               Puntero a la estructura del comando a actualizar.
 *
 * @param fd     The new file descriptor to set. /
 *               El nuevo descriptor de archivo a establecer.
 *
 * @param redir  The redirection operator (e.g., "<", ">", ">>", "<<"). /
 *               El operador de redirección (p.ej., "<", ">", ">>", "<<").
 *
 * @returns 0 on success. /
 *          0 en éxito.
 */
static int	ft_update_fds(t_cmd *cmd, int fd, char *redir)
{
	if (ft_strcmp(redir, "<") == 0 || ft_strcmp(redir, "<<") == 0)
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
	return (0);
}

/**
 * ENGLISH: Opens the appropriate file descriptor based on the redirection type.
 *
 * SPANISH: Abre el descriptor de archivo apropiado según el tipo de redirección.
 *
 * @param redir  The redirection operator (e.g., "<", ">", ">>", "<<"). /
 *               El operador de redirección (p.ej., "<", ">", ">>", "<<").
 *
 * @param arg    The filename or delimiter associated with the redirection. /
 *               El nombre del archivo o delimitador asociado con la redirección.
 *
 * @returns The opened file descriptor on success, -1 on error. /
 *          El descriptor de archivo abierto en éxito, -1 en error.
 */
static int	ft_open_redir_fd(char *redir, char *arg)
{
	if (ft_strcmp(redir, "<") == 0)
		return (ft_handle_infile(arg));
	else if (ft_strcmp(redir, ">") == 0 || ft_strcmp(redir, ">>") == 0)
		return (ft_handle_outfile(arg, ft_strcmp(redir, ">>") == 0));
	else
		return (ft_handle_heredoc(arg));
}

/**
 * ENGLISH: Handles a single redirection operation for a command, updating file
 *          descriptors and error status as needed.
 *
 * SPANISH: Maneja una sola operación de redirección para un comando,
 *          actualizando los descriptores de archivo y el estado de error
 *          según sea necesario.
 *
 * @param cmd     Pointer to the command structure to update. /
 *                Puntero a la estructura del comando a actualizar.
 *
 * @param argv    Argument vector containing redirection operators and
 *                filenames. /
 *                Vector de argumentos que contiene operadores de redirección y
 *                nombres de archivo.
 *
 * @param i       Index in argv where the redirection operator is found. /
 *                Índice en argv donde se encuentra el operador de redirección.
 *
 * @param data    Pointer to shell data structure for updating exit status. /
 *                Puntero a la estructura de datos del shell para actualizar
 *                el estado de salida.
 *
 * @returns The next index to process in argv. /
 *          El siguiente índice a procesar en argv.
 */
int	ft_handle_redirection(t_cmd *cmd, char **argv, int i, t_data *data)
{
	char	*clean_arg;
	int		fd;

	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	fd = ft_open_redir_fd(argv[i], clean_arg);
	if (fd != -1)
		ft_update_fds(cmd, fd, argv[i]);
	else
	{
		data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (i + 1);
}
