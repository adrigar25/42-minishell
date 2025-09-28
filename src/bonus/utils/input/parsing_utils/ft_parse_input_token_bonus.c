/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_token_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:55:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:43:16 by adriescr         ###   ########.fr       */
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
	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "||") == 0
		|| ft_strcmp(token, "&&") == 0 || ft_strcmp(token, "&") == 0);
}

/**
 * ENGLISH: Processes a single token from the input arguments,
 *          updating the current command structure accordingly.
 *          Handles pipes, logical operators, redirections, and arguments.
 *
 * SPANISH: Procesa un solo token de los argumentos de entrada,
 *          actualizando la estructura del comando actual en consecuencia.
 *          Maneja tuberías, operadores lógicos, redirecciones y argumentos.
 *
 * @param current_cmd Pointer to the current command pointer. /
 *                    Puntero al puntero del comando actual.
 *
 * @param argv        The array of input argument tokens. /
 *                    El arreglo de tokens de argumentos de entrada.
 *
 * @param i           The current index in the argv array. /
 *                    El índice actual en el arreglo argv.
 *
 * @param cmd_index   Pointer to the current command index. /
 *                    Puntero al índice del comando actual.
 *
 * @returns The next index to process in the argv array. /
 *          El siguiente índice a procesar en el arreglo argv.
 */
int	ft_process_token(t_cmd **current_cmd, char **argv, int i, int *cmd_index)
{
	char	*clean_arg;
	t_cmd	*new_cmd;
	int		k;
	int		count_nonop;

	k = 0;
	if (is_pipe_like_token(argv[i]))
	{
		ft_process_op(current_cmd, argv[i], cmd_index, (*current_cmd)->data);
		return (i + 1);
	}
	else if (is_redir_token(argv[i]) && (*current_cmd)->has_error == 0)
	{
		return (ft_handle_redirection(*current_cmd, argv, i,
				(*current_cmd)->data));
	}
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (clean_arg)
			ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i + 1);
	}
	return (i + 1);
}
