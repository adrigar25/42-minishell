/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 20:29:05 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/20 19:22:52 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

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
		fd = ft_heredoc(filename, (*cmd)->data);
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
/* declarations: implemented in ft_redir_error_bonus.c */
int	ft_is_ambiguous_redirect(char *filename);
int	ft_check_ambiguous(t_cmd *cmd, char **argv, int i, char *expanded_arg);
int	ft_handle_fd_error(t_cmd *cmd, int fd_ret);
int	ft_handle_wildcard_sentinel(t_cmd *cmd, char *expanded_arg);

int	ft_redir(t_cmd *cmd, char **argv, int i)
{
	char	*expanded_arg;
	char	*clean_arg;
	int		fd_ret;

	expanded_arg = ft_process_arg(argv[i + 1], cmd->data);
	if (ft_handle_wildcard_sentinel(cmd, expanded_arg))
		return (i++);
	if (ft_check_ambiguous(cmd, argv, i, expanded_arg))
		return (i++);
	if (cmd->has_error == 1 && ft_strcmp(argv[i], "<<") != 0)
		return (free(expanded_arg), i++);
	clean_arg = ft_remove_quotes(expanded_arg);
	if (!clean_arg)
		clean_arg = expanded_arg;
	fd_ret = ft_assign_fd(&cmd, clean_arg, argv[i]);
	if (clean_arg != expanded_arg)
		free(clean_arg);
	free(expanded_arg);
	if (ft_handle_fd_error(cmd, fd_ret) == -1)
		return (-1);
	return (i + 1);
}
