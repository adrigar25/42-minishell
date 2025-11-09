/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 12:20:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

/**
 * ENGLISH: Assigns a file descriptor to the command structure based on
 *          the redirection type.
 *
 * SPANISH: Asigna un descriptor de archivo a la estructura del comando
 *          según el tipo de redirección.
 *
 * @param cmd       Pointer to the command structure to update.
 *                  Puntero a la estructura de comando a actualizar.
 *
 * @param filename  The filename for the redirection.
 *                  El nombre del archivo para la redirección.
 *
 * @param type      The type of redirection ("<", ">", ">>", "<<").
 *                  El tipo de redirección ("<", ">", ">>", "<<").
 *
 * @returns The file descriptor on success, -1 on error.
 *          El descriptor de archivo en caso de éxito, -1 en caso de error.
 */
int	ft_assign_fd(t_cmd **cmd, char *filename, char *type)
{
	int	fd;

	fd = -1;
	if (ft_strcmp(type, "<") == 0)
		fd = ft_handle_infile(filename);
	else if (!ft_strcmp(type, ">") || !ft_strcmp(type, ">>"))
		fd = ft_handle_outfile(filename, 0 + (ft_strcmp(type, ">>") == 0));
	else if (ft_strcmp(type, "<<") == 0)
		fd = ft_heredoc(filename, (*cmd)->data, 1);
	if (fd == -1)
		return (-1);
	if (ft_strcmp(type, "<") == 0 || ft_strcmp(type, "<<") == 0)
	{
		if ((*cmd)->infd != STDIN_FILENO)
			close((*cmd)->infd);
		(*cmd)->infd = fd;
	}
	else
	{
		if ((*cmd)->outfd != STDOUT_FILENO)
			close((*cmd)->outfd);
		(*cmd)->outfd = fd;
	}
	return (fd);
}

/**
 * ENGLISH: Checks if a token is a redirection operator.
 *
 * SPANISH: Verifica si un token es un operador de redirección.
 *
 * @param token The token to check.
 *              El token a verificar.
 *
 * @returns 1 if the token is a redirection operator, 0 otherwise.
 *          1 si el token es un operador de redirección, 0 en caso contrario.
 */
int	ft_redir(t_cmd *cmd, char **argv, int i)
{
	char	*clean_arg;

	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	/* If a previous redirection set has_error, avoid trying to open files
	   for normal redirections (>, >>, <). Still process heredoc (<<). This
	   ensures we consume the filename token but don't emit duplicate errors. */
	if (cmd->has_error == 1 && ft_strcmp(argv[i], "<<") != 0)
	{
		if (clean_arg != argv[i + 1])
			free(clean_arg);
		return (i + 1);
	}
	int fd_ret = ft_assign_fd(&cmd, clean_arg, argv[i]);
	if (fd_ret == -2)
	{
		/* heredoc was aborted by SIGINT: signal parser to abort input */
		if (clean_arg != argv[i + 1])
			free(clean_arg);
		return (-1);
	}
	if (fd_ret == -1)
	{
		cmd->data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (i + 1);
}
