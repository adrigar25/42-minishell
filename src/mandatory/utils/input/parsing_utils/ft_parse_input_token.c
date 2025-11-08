/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:55:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/07 23:20:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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
int	ft_process_token(t_cmd **current_cmd, char **argv, int i, int *cmd_index)
{
	char	*clean_arg;

	if (!argv[i])
		return (i);
	if (ft_strcmp(argv[i], "|") == 0)
		return (ft_process_pipe(current_cmd, cmd_index, (*current_cmd)->data),
			i);
	else if ((!ft_strcmp(argv[i], "<") || !ft_strcmp(argv[i], ">")
			|| !ft_strcmp(argv[i], ">>") || !ft_strcmp(argv[i], "<<"))
		&& (*current_cmd)->has_error == 0)
		return (ft_redir(*current_cmd, argv, i));
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (!clean_arg)
			return (i);
		ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i);
	}
}
