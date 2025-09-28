/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_error_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:27:37 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:50:33 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../mandatory/minishell.h"

/**
 * ENGLISH: Prints execution-related error messages based on the error code.
 *
 * SPANISH: Imprime mensajes de error relacionados con la ejecución según
 * 			el código de error.
 *
 * @param code   The execution error code to handle. /
 *               El código de error de ejecución a manejar.
 *
 * @param msg    The message argument for formatting (e.g., command name). /
 *               El argumento de mensaje para formatear (por ejemplo, nombre
 * 				del comando).
 *
 * Handles the following error codes:
 *   1 - Command not found
 *   2 - Permission denied
 *   3 - Is a directory
 *   4 - No such file
 */
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

/**
 * ENGLISH: Prints syntax-related error messages based on the error code.
 *
 * SPANISH: Imprime mensajes de error relacionados con la sintaxis según
 * 			el código de error.
 *
 * @param code   The syntax error code to handle. /
 * 			 El código de error de sintaxis a manejar.
 * @param msg    The message argument for formatting (e.g., unexpected token). /
 * 			 El argumento de mensaje para formatear (por ejemplo, token
 * 			inesperado).
 *
 * Handles the following error codes:
 *  5 - Syntax error near unexpected token
 *  6 - Syntax error: unexpected token
 */
static void	print_syntax_errors(int code, char *msg)
{
	if (code == 5)
		ft_fprintf(2, ERROR_SYNTAX);
	else if (code == 6)
		ft_fprintf(2, ERROR_SYNTAX_TOKEN, msg);
}

/**
 * ENGLISH: Prints built-in command and environment-related error messages
 *		  based on the error code.
 *
 * SPANISH: Imprime mensajes de error relacionados con comandos integrados y
 *		  entorno según el código de error.
 *
 * @param code   The built-in/env error code to handle. /
 * 			 El código de error de comandos integrados/entorno a manejar.
 * @param msg    The first message argument for formatting. /
 * 			 El primer argumento de mensaje para formatear.
 * @param msg2   The second message argument for formatting. /
 * 			 El segundo argumento de mensaje para formatear.
 *
 * Handles the following error codes:
 * 7  - Too many arguments
 * 8  - HOME not set
 * 11 - cd command failure
 * 12 - Ambiguous redirect
 * 13 - Invalid identifier
 */
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

/**
 * ENGLISH: Prints redirection-related error messages based on the error code.
 * 			Handles heredoc and ambiguous redirect errors.
 *
 * SPANISH: Imprime mensajes de error relacionados con redirecciones según
 * 			el código de error. Maneja errores de heredoc y redirección
 * 			ambigua.
 *
 * @param code   The redirection error code to handle. /
 * 			 El código de error de redirección a manejar.
 * @param msg    The message argument for formatting (e.g., filename). /
 * 		 El argumento de mensaje para formatear (por ejemplo, nombre de archivo).
 *
 * Handles the following error codes:
 * 9  - Heredoc delimiter error
 * 10 - Heredoc process error
 * 12 - Ambiguous redirect
 */
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
	if (error_code >= 1 && error_code <= 4)
		print_exec_errors(error_code, msg);
	else if (error_code >= 5 && error_code <= 6)
		print_syntax_errors(error_code, msg);
	else if (error_code >= 9 && error_code <= 12)
		print_redir_errors(error_code, msg);
	else if (error_code >= 7 && error_code <= 14)
		print_builtin_env_errors(error_code, msg, msg2);
	return (exit_code);
}
