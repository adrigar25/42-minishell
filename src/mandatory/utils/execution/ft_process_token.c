/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:49:41 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/26 21:59:27 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Handles the pipe token by creating a new command node,
 * 		setting up the necessary file descriptors for piping.
 * 		Closes unused file descriptors to prevent leaks.
 * 	 Updates the current command pointer to the new command node.
 * SPANISH: Maneja el token de tubería creando un nuevo nodo de comando,
 * 	 configurando los descriptores de archivo necesarios para la tubería.
 * 	 Cierra los descriptores de archivo no utilizados para prevenir fugas.
 * 	 Actualiza el puntero del comando actual al nuevo nodo de comando.
 *
 * @param current_cmd  Pointer to the current command node pointer. /
 * 				   Puntero al puntero del nodo de comando actual.
 * @param cmd_index    Pointer to the command index counter. /
 * 				   Puntero al contador de índice de comando.
 * @param data         Pointer to the shell's main data structure. /
 * 				   Puntero a la estructura principal de datos del shell.
 * @returns 0 on success, -1 on failure. /
 * 		   0 en éxito, -1 en fallo.
 */
static int	ft_handle_pipe(t_cmd **current_cmd, int *cmd_index, t_data *data)
{
	int		pipefd[2];
	t_cmd	*new_cmd;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	new_cmd = ft_create_cmd_node(*cmd_index + 1);
	if (!new_cmd)
		return (-1);
	(*cmd_index)++;
	(*current_cmd)->next = new_cmd;
	new_cmd->data = data;
	if ((*current_cmd)->outfd == STDOUT_FILENO)
		(*current_cmd)->outfd = pipefd[1];
	else
		close(pipefd[1]);
	new_cmd->infd = pipefd[0];
	*current_cmd = new_cmd;
	return (0);
}

/**
 * ENGLISH: Processes a token from the command arguments.
 * 	 If the token is a pipe, it handles the pipe setup.
 * 	 If it's a redirection operator, it processes the redirection.
 * 	 Otherwise, it treats the token as a command argument.
 * SPANISH: Procesa un token de los argumentos del comando.
 * 	 Si el token es una tubería, maneja la configuración de la tubería.
 * 	 Si es un operador de redirección, procesa la redirección.
 * 	 De lo contrario, trata el token como un argumento del comando.
 *
 * @param current_cmd  Pointer to the current command node pointer. /
 * 				   Puntero al puntero del nodo de comando actual.
 * @param argv         Array of command argument tokens. /
 * 				   Array de tokens de argumentos del comando.
 * @param i            Current index in the argv array. /
 * 				   Índice actual en el array argv.
 * @param cmd_index    Pointer to the command index counter. /
 * 				   Puntero al contador de índice de comando.
 * @param data         Pointer to the shell's main data structure. /
 * 				   Puntero a la estructura principal de datos del shell.
 * @returns Updated index after processing the token. /
 * 		   Índice actualizado después de procesar el token.
 */
int	ft_process_token(t_cmd **current_cmd, char **argv, int i,
		int *cmd_index, t_data *data)
{
	char	*clean_arg;

	if (ft_strcmp(argv[i], "|") == 0)
	{
		if (ft_handle_pipe(current_cmd, cmd_index, data) == -1)
			return (i);
		return (i);
	}
	else if ((ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
			|| ft_strcmp(argv[i], ">>") == 0 || ft_strcmp(argv[i], "<<") == 0)
		&& (*current_cmd)->has_error == 0)
	{
		return (ft_handle_redirection(*current_cmd, argv, i, data));
	}
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (clean_arg)
			ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i);
	}
}
