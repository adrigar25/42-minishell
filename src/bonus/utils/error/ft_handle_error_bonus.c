/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_error_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:27:37 by agarcia           #+#    #+#             */
/*   Updated: 2025/10/30 00:51:40 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../mandatory/minishell.h"

static void	print_exec_errors(int code, char *msg)
{
	if (code == 1)
		ft_fprintf(2, ERROR_COMMAND_NOT_FOUND, msg);
	else if (code == 2)
		ft_fprintf(2, ERROR_PERMISSION_DENIED);
	else if (code == 3)
		ft_fprintf(2, ERROR_IS_A_DIRECTORY, msg);
	else if (code == 4)
		ft_fprintf(2, ERROR_NO_SUCH_FILE, msg);
}

static void	print_syntax_errors(int code, char *msg)
{
	if (code == 5)
		ft_fprintf(2, ERROR_SYNTAX);
	else if (code == 6)
		ft_fprintf(2, ERROR_SYNTAX_TOKEN, msg);
}

static void	print_builtin_env_errors(int code, char *msg, char *msg2)
{
	if (code == 7)
		ft_fprintf(2, ERROR_TOO_MANY_ARGS);
	else if (code == 8)
		ft_fprintf(2, ERROR_HOME_NOT_SET);
	else if (code == 11)
		ft_fprintf(2, ERROR_CD_FAIL, msg, msg2);
	else if (code == 13)
		ft_fprintf(2, ERROR_INVALID_IDENTIFIER, msg);
	else if (code == 14)
		ft_fprintf(2, ERROR_NUM_ARG_REQ, msg);
}

static void	print_redir_errors(int code, char *msg)
{
	if (code == 9)
		ft_fprintf(2, ERROR_HEREDOC_DELIMITER);
	else if (code == 10)
		ft_fprintf(2, ERROR_HEREDOC_PROCESS);
	else if (code == 12)
		ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, msg);
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
	/* 1-4: ejecución */
	if (error_code >= 1 && error_code <= 4)
		print_exec_errors(error_code, msg);
	/* 5-6: sintaxis */
	else if (error_code >= 5 && error_code <= 6)
		print_syntax_errors(error_code, msg);
	/* 9,10,12: redirecciones (heredoc y ambiguous) */
	else if (error_code == 9 || error_code == 10 || error_code == 12)
		print_redir_errors(error_code, msg);
	/* 7,8,11,13,14: builtins/env */
	else if (error_code == 7 || error_code == 8 || error_code == 11
		|| error_code == 13 || error_code == 14)
		print_builtin_env_errors(error_code, msg, msg2);
	return (exit_code);
}
