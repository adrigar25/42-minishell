/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:27:37 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 15:35:49 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	print_error_message_part2(int error_code, char *msg, char *msg2)
{
	if (error_code == 9)
		ft_fprintf(2, ERROR_HEREDOC_DELIMITER);
	else if (error_code == 10)
		ft_fprintf(2, ERROR_HEREDOC_PROCESS);
	else if (error_code == 11)
		ft_fprintf(2, ERROR_CD_FAIL, msg, msg2);
	else if (error_code == 12)
		ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, msg);
	else if (error_code == 13)
		ft_fprintf(2, ERROR_INVALID_IDENTIFIER, msg);
	else if (error_code == 14)
		ft_fprintf(2, ERROR_NUM_ARG_REQ, msg);
}

static void	print_error_message_part1(int error_code, char *msg, char *msg2)
{
	if (error_code == 1)
		ft_fprintf(2, ERROR_COMMAND_NOT_FOUND, msg);
	else if (error_code == 2)
		ft_fprintf(2, ERROR_PERMISSION_DENIED);
	else if (error_code == 3)
		ft_fprintf(2, ERROR_IS_A_DIRECTORY, msg);
	else if (error_code == 4)
		ft_fprintf(2, ERROR_NO_SUCH_FILE, msg);
	else if (error_code == 5)
		ft_fprintf(2, ERROR_SYNTAX);
	else if (error_code == 6)
		ft_fprintf(2, ERROR_SYNTAX_TOKEN, msg);
	else if (error_code == 7)
		ft_fprintf(2, ERROR_TOO_MANY_ARGS);
	else if (error_code == 8)
		ft_fprintf(2, ERROR_HOME_NOT_SET);
	else
		print_error_message_part2(error_code, msg, msg2);
}

/**
 * ENGLISH: Handles error codes by printing the corresponding error message
 *          and returns the provided exit code.
 *
 * SPANISH: Maneja los códigos de error imprimiendo el mensaje de error
 *          correspondiente y devuelve el código de salida proporcionado.
 *
 * @param error_code   The error code to handle. /
 *                     El código de error a manejar.
 *
 * @param exit_code    The exit code to return. /
 *                     El código de salida a devolver.
 *
 * @param msg          The first message argument for formatting. /
 *                     El primer argumento de mensaje para formatear.
 *
 * @param msg2         The second message argument for formatting. /
 *                     El segundo argumento de mensaje para formatear.
 *
 * @returns The provided exit code. /
 *          El código de salida proporcionado.
 */
int	ft_handle_error(int error_code, int exit_code, char *msg, char *msg2)
{
	print_error_message_part1(error_code, msg, msg2);
	return (exit_code);
}
