/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:59:01 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Expands an environment variable in the given argument string,
 *          appending its value to the destination string.
 *
 * SPANISH: Expande una variable de entorno en la cadena de argumentos dada,
 *          añadiendo su valor a la cadena de destino.
 *
 * @param dst   Pointer to the destination string to append the variable
 *              value to. /
 *              Puntero a la cadena de destino donde se añadirá el valor
 *              de la variable.
 *
 * @param arg   The argument string containing the environment variable. /
 *              La cadena de argumentos que contiene la variable de entorno.
 *
 * @param j     Pointer to the current index in the argument string
 *              (points to '$'). /
 *              Puntero al índice actual en la cadena de argumentos
 *              (apunta a '$').
 *
 * @param data  Pointer to the shell data structure containing environment
 *              variables. /
 *              Puntero a la estructura de datos del shell que contiene las
 *              variables de entorno.
 *
 * @returns 1 on success, 0 on memory allocation failure. /
 *          1 en caso de éxito, 0 en caso de fallo de asignación de memoria.
 */
int	ft_expand_env_var(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
	start = *j;
	while (arg[*j] && (ft_isalnum(arg[*j]) || arg[*j] == '_'))
		(*j)++;
	if (*j == start)
		return (ft_append(dst, "$"));
	env_name = ft_substr(arg, start, *j - start);
	if (!env_name)
		return (0);
	env_value = ft_getenv(env_name, data->envp);
	free(env_name);
	if (env_value)
		if (!ft_append(dst, (char *)env_value))
			return (0);
	return (1);
}
