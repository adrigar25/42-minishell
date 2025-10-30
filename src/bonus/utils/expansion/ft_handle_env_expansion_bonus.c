/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/10/30 01:37:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Handles environment variable expansion for an array of argument
 *          strings, returning a new array with expanded values.
 *
 * SPANISH: Maneja la expansión de variables de entorno para un arreglo de
 *          cadenas de argumentos, devolviendo un nuevo arreglo con los
 *          valores expandidos.
 *
 * @param argv  The original array of argument strings. /
 *              El arreglo original de cadenas de argumentos.
 *
 * @param data  Pointer to the shell data structure containing environment
 *              variables. /
 *              Puntero a la estructura de datos del shell que contiene las
 *              variables de entorno.
 *
 * @returns A new array of argument strings with expanded environment variables,
 *          or the original array on memory allocation failure. /
 *          Un nuevo arreglo de cadenas de argumentos con las variables de
 *          entorno expandidas, o el arreglo original en caso de fallo de
 *          asignación de memoria.
 */
char	**ft_handle_env_expansion(char **argv, t_data *data)
{
	int		i;
	char	**new_argv;

	if (!argv || !data || !data->envp)
		return (argv);
	new_argv = malloc(sizeof(char *) * (data->argc + 1));
	if (!new_argv)
		return (argv);
	i = 0;
	while (argv[i])
	{
		new_argv[i] = ft_strdup("");
		if (!new_argv[i] || !ft_process_arg(&new_argv[i], argv[i], data))
		{
			ft_free_matrix(new_argv);
			return (argv);
		}
		i++;
	}
	new_argv[i] = NULL;
	/* Free original argv on success to avoid leaks */
	ft_free_matrix(argv);
	return (new_argv);
}
