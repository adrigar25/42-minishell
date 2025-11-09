/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 12:20:58 by agarcia          ###   ########.fr       */
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

	fd = 0;
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
 * ENGLISH: Checks for ambiguous redirection scenarios.
 *          A redirection is considered ambiguous if it uses wildcards
 *          and matches zero or multiple files, except for heredoc ('<<').
 *
 * SPANISH: Verifica escenarios de redirección ambiguos.
 *          Una redirección se considera ambigua si utiliza comodines
 *          y coincide con cero o múltiples archivos, excepto para
 *          heredoc ('<<').
 *
 * @param argv  The array of argument strings.
 *              El arreglo de cadenas de argumentos.
 *
 * @param i     The current index in the argument array.
 *              El índice actual en el arreglo de argumentos.
 *
 * @returns 1 if the redirection is ambiguous, 0 otherwise.
 *          1 si la redirección es ambigua, 0 en caso contrario.
 */
static int	is_ambiguous_redir(char **argv, int i)
{
	int	matches;

	if (!argv[i + 1])
		return (0);
	if (ft_strcmp(argv[i], "<<") == 0)
		return (0);
	matches = ft_count_matches(argv[i + 1]);
	if (ft_has_wildcards(argv[i + 1]) && matches != 1)
		return (1);
	return (0);
}

/**
 * ENGLISH: Handles input/output redirection for a command based on the
 *          provided arguments. It checks for ambiguous redirections,
 *          removes quotes from filenames, retrieves file descriptors,
 *          and assigns them to the command structure.
 *
 * SPANISH: Maneja la redirección de entrada/salida para un comando
 *          basado en los argumentos proporcionados. Verifica redirecciones
 *          ambiguas, elimina comillas de los nombres de archivo, obtiene
 *          descriptores de archivo y los asigna a la estructura del comando.
 *
 * @param cmd   Pointer to the command structure to update.
 *              Puntero a la estructura de comando a actualizar.
 *
 * @param argv  The array of argument strings.
 *              El arreglo de cadenas de argumentos.
 *
 * @param i     The current index in the argument array
 *              (points to the redirection operator).
 *              El índice actual en el arreglo de argumentos
 *              (apunta al operador de redirección).
 *
 * @param data  Pointer to the shell data structure containing
 *              environment variables and last exit status.
 *              Puntero a la estructura de datos del shell que contiene
 *              las variables de entorno y el último estado de salida.
 *
 * @returns The next index to process in the argument array after
 *          handling the redirection.
 *          El siguiente índice a procesar en el arreglo de argumentos
 *          después de manejar la redirección.
 */
int	ft_redir(t_cmd *cmd, char **argv, int i)
{
	char	*redir;

	if (is_ambiguous_redir(argv, i) == 1)
	{
		cmd->data->last_exit_status = ft_handle_error(12, 1, argv[i + 1], NULL);
		cmd->has_error = 1;
		/*
		** Match the mandatory version's return convention: return the index
		** of the filename token (i + 1). The outer loop will advance to
		** the token after that (i + 2). Returning i + 2 here caused the
		** parser to skip the token that follows the redirection filename.
		*/
		return (i + 1);
	}
	redir = ft_remove_quotes(argv[i + 1]);
	if (!redir)
		redir = argv[i + 1];
	/* If a previous redirection set has_error, avoid trying to open files
		for normal redirections (>, >>, <). Still process heredoc (<<). This
		ensures we consume the filename token but don't emit duplicate errors. */
	if (cmd->has_error == 1 && ft_strcmp(argv[i], "<<") != 0)
	{
		if (redir != argv[i + 1])
			free(redir);
		return (i + 1);
	}
	int fd_ret = ft_assign_fd(&cmd, redir, argv[i]);
	if (fd_ret == -2)
	{
		if (redir != argv[i + 1])
			free(redir);
		return (i + 1 == 0 ? -1 : -1);
	}
	if (fd_ret == -1)
	{
		cmd->data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (redir != argv[i + 1])
		free(redir);
	/*
	** Return the index of the filename token so the caller's loop will
	** advance to the next token after the filename. See comment above.
	*/
	return (i + 1);
}
