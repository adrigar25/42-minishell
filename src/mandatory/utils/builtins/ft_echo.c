/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 18:16:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Prints a single argument for the echo command, handling
 * 			quoted sections and the -n flag.
 *
 * SPANISH: Imprime un solo argumento para el comando echo, manejando
 * 			secciones entre comillas y la bandera -n.
 *
 * @param arg    The argument string to print. /
 *               La cadena de argumentos a imprimir.
 *
 * @param flag_n If set to 1, indicates that the -n flag is active
 *               (no trailing newline). /
 *               Si se establece en 1, indica que la bandera -n está activa
 *               (sin nueva línea final).
 *
 * @param outfd  The file descriptor to write the output to. /
 *               El descriptor de archivo donde escribir la salida.
 *
 * @returns 0 on success, -1 on error. /
 *          0 en caso de éxito, -1 en caso de error.
 */
static int	ft_putarg_echo(char *arg, int flag_n, int outfd)
{
	int	start;
	int	end;

	if (!arg)
		return (0);
	ft_handle_quoted_arg(arg, &start, &end);
	if (ft_print_arg_content(arg, start, end, outfd) == -1)
		return (-1);
	if (flag_n && arg[end - 1] == ' ' && ft_putchar_fd('%', outfd) == -1)
		return (-1);
	return (0);
}

/**
 * ENGLISH: Implements the echo command, printing arguments to the specified
 * 			output file descriptor, handling the -n flag and spacing.
 *
 * SPANISH: Implementa el comando echo, imprimiendo argumentos en el
 * 			descriptor de archivo de salida especificado, manejando la
 * 			bandera -n y los espacios.
 *
 * @param cmd The command structure containing arguments and file descriptors. /
 *            La estructura de comando que contiene argumentos y descriptores
 * 				de archivo.
 *
 * @returns 0 on success, 1 on error. /
 *          0 en caso de éxito, 1 en caso de error.
 */
static int	ft_intflag_n(char *arg)
{
	if (arg && arg[0] == '-' && arg[1] == 'n')
		return (1);
	return (0);
}

/**
 * ENGLISH: Prints all arguments for the echo command, handling spacing
 * 			between arguments.
 *
 * SPANISH: Imprime todos los argumentos para el comando echo, manejando
 * 			los espacios entre argumentos.
 *
 * @param cmd         The command structure containing arguments and file
 * 						descriptors. /
 *                    La estructura de comando que contiene argumentos y
 * 						descriptores de archivo.
 *
 * @param start_index The index to start printing arguments from. /

	*                    El índice desde el cual comenzar a imprimir los argumentos.
 *
 * @param outfd       The file descriptor to write the output to. /
 *                    El descriptor de archivo donde escribir la salida.
 *
 * @returns 0 on success, 1 on error. /
 *          0 en caso de éxito, 1 en caso de error.
 */
static int	ft_print_echo_args(t_cmd cmd, int start_index, int outfd)
{
	int	i;

	i = start_index;
	while (cmd.argv[i])
	{
		if (ft_putarg_echo(cmd.argv[i], 0, outfd) == -1)
			return (1);
		if (cmd.argv[i + 1] && cmd.argv[i + 1][0] != '\0' && ft_putchar_fd(' ',
				outfd) == -1)
			return (1);
		i++;
	}
	return (0);
}

/**
 * ENGLISH: Implements the echo command, printing arguments to the specified
 * 			output file descriptor, handling the -n flag and spacing.
 *
 * SPANISH: Implementa el comando echo, imprimiendo argumentos en el
 * 			descriptor de archivo de salida especificado, manejando la
 * 			bandera -n y los espacios.
 *
 * @param cmd The command structure containing arguments and file descriptors. /
 *            La estructura de comando que contiene argumentos y descriptores
 * 				de archivo.
 *
 * @returns 0 on success, 1 on error. /
 *          0 en caso de éxito, 1 en caso de error.
 */
int	ft_echo(t_cmd cmd)
{
	int	n_flag;
	int	start_index;

	if (!cmd.argv || !cmd.argv[0] || cmd.outfd < 0)
		return (1);
	n_flag = 0;
	start_index = 1;
	if (cmd.argv[1] && ft_intflag_n(cmd.argv[1]))
	{
		n_flag = 1;
		start_index = 2;
	}
	if (ft_print_echo_args(cmd, start_index, cmd.outfd) || (!n_flag
			&& ft_putchar_fd('\n', cmd.outfd) == -1))
		return (1);
	return (0);
}
