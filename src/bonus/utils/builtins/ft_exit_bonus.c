/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:46:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Converts a string to a long long integer.
 * 		Handles optional leading '+' or '-' signs.
 * 	SPANISH: Convierte una cadena a un entero largo largo.
 * 		Maneja signos '+' o '-' opcionales al principio.
 *
 * @param str   The string to convert. /
 * 			La cadena a convertir.
 *
 * @return The converted long long integer. /
 * 			El entero largo largo convertido.
 */
long long	ft_atoll(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	count_exit_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static void	handle_non_numeric_arg(char *arg)
{
	ft_fprintf(2, "exit\n");
	exit(ft_handle_error(14, 2, arg, NULL));
}

static void	exit_with_numeric_code(char *arg)
{
	long long	exit_code;

	exit_code = ft_atoll(arg);
	exit((int)(exit_code % 256));
}

/**
 * ENGLISH: Implements the exit command, handling optional numeric arguments
 * 			and error cases.
 *
 * SPANISH: Implementa el comando exit, manejando argumentos numéricos
 * 			opcionales y casos de error.
 *
 * @param cmd The command structure containing arguments. /
 *            La estructura de comando que contiene los argumentos.
 *
 * @returns This function does not return ; it exits the program. /
 *          Esta función no retorna; sale del programa.
 */
int	ft_exit(t_cmd *cmd)
{
	char	**args;
	int		argc;

	args = cmd->argv;
	if (!args)
		exit(0);
	argc = count_exit_args(args);
	if (argc == 1)
		exit(0);
	else if (argc > 2)
	{
		ft_fprintf(2, "exit\n");
		return (ft_handle_error(7, EXIT_FAILURE, NULL, NULL));
	}
	else if (!ft_is_numeric(args[1]))
		handle_non_numeric_arg(args[1]);
	else
		exit_with_numeric_code(args[1]);
	return (0);
}
