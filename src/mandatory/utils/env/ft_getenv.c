/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:26:32 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 16:32:05 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Retrieves the value of an environment variable from the
 * 			environment array.
 *
 * SPANISH: Recupera el valor de una variable de entorno del array de entorno.
 *
 * @param name   The name of the environment variable to retrieve. /
 *               El nombre de la variable de entorno a recuperar.
 *
 * @param envp   The environment variable array. /
 *               El array de variables de entorno.
 *
 * @returns The value of the environment variable, or NULL if not found. /
 *          El valor de la variable de entorno, o NULL si no se encuentra.
 */
char	*ft_getenv(const char *name, char **envp)
{
	int		i;
	char	*var;

	if (!name || !envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		var = ft_strchr(envp[i], '=');
		if (var)
		{
			if (ft_strncmp(envp[i], name, var - envp[i]) == 0)
				return (var + 1);
		}
		else
		{
			if (ft_strcmp(envp[i], name) == 0)
				return ("");
		}
		i++;
	}
	return (NULL);
}
