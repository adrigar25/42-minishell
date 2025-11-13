/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/12 16:47:44 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_append_dollar_sign(char **dst)
{
	if (!ft_append(dst, "$"))
		return (-1);
	return (1);
}

/**
 * ENGLISH: Expands an environment variable found in the argument string.
 *          Appends the variable's value to the destination string.
 *
 * SPANISH: Expande una variable de entorno encontrada en la cadena de
 * 			argumentos. Añade el valor de la variable a la cadena de
 * 			destino.
 *
 * @param dst   Pointer to the destination string to append the value to. /
 *              Puntero a la cadena de destino donde añadir el valor.
 *
 * @param arg   The argument string containing the environment variable. /
 *              La cadena de argumentos que contiene la variable de entorno.
 *
 * @param j     Pointer to the current index in the argument string. /
 *              Puntero al índice actual en la cadena de argumentos.
 *
 * @param data  Pointer to the data structure containing environment variables. /
 *              Puntero a la estructura de datos que contiene las variables
 *              de entorno.
 *
 * @returns 1 on successful expansion, 0 if variable not found, -1 on error. /
 *          1 en caso de expansión exitosa, 0 si no se encuentra la variable,
 *          -1 en caso de error.
 */
int	ft_expand_env_var(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
	if (arg[*j] == '\'' || arg[*j] == '\"')
		return (ft_append_dollar_sign(dst));
	start = *j;
	while (arg[*j] && (ft_isalnum(arg[*j]) || arg[*j] == '_'))
		(*j)++;
	if (*j == start)
		return (ft_append_dollar_sign(dst));
	env_name = ft_substr(arg, start, *j - start);
	if (!env_name)
		return (-1);
	env_value = ft_getenv(env_name, data->envp);
	free(env_name);
	if (!env_value || env_value[0] == '\0')
		return (0);
	if (!ft_append(dst, env_value))
		return (-1);
	return (1);
}
