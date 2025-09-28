/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_redir_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:41:34 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

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
static int	is_redir_token(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

/**
 * ENGLISH: Checks if a token is a pipe or logical operator.
 *
 * SPANISH: Verifica si un token es un operador de tubería o lógico.
 *
 * @param token The token to check.
 * 			El token a verificar.
 *
 * @returns 1 if the token is a pipe or logical operator, 0 otherwise.
 * 			1 si el token es un operador de tubería o lógico, 0 en
 * 			caso contrario.
 */
static int	is_pipe_like_token(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "|") || !ft_strcmp(token, "||")
		|| !ft_strcmp(token, "&&"));
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
 * @param data  Pointer to the shell data structure containing
 *              environment variables.
 *              Puntero a la estructura de datos del shell que
 *              contiene las variables de entorno.
 *
 * @returns 1 if the redirection is ambiguous, 0 otherwise.
 *          1 si la redirección es ambigua, 0 en caso contrario.
 */
static int	is_ambiguous_redir(char **argv, int i, t_data *data)
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
int	ft_handle_redirection(t_cmd *cmd, char **argv, int i, t_data *data)
{
	char	*clean_arg;
	int		fd;
	int		k;
	int		count_nonop;

	if (is_ambiguous_redir(argv, i, data) == 1)
	{
		data->last_exit_status = ft_handle_error(12, 1, argv[i + 1], NULL);
		cmd->has_error = 1;
		return (i + 2);
	}
	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	fd = ft_get_fd_from_token(argv[i], clean_arg);
	if (fd != -1)
		ft_assign_fd(cmd, argv[i], fd);
	else
	{
		data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (i + 2);
}
