/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/08 02:12:05 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Implements the env command, printing all environment variables
 * 			to the specified output file descriptor.
 *
 * SPANISH: Implementa el comando env, imprimiendo todas las variables de
 * 			entorno en el descriptor de archivo de salida especificado.
 *
 * @param cmd  The command structure containing file descriptors. /
 *             La estructura de comando que contiene descriptores de archivo.
 * @param envp The environment variables array. /
 *             La matriz de variables de entorno.
 *
 * @returns 0 on success, 1 on error. /
 *          0 en caso de éxito, 1 en caso de error.
 */
int	ft_env(t_cmd cmd, char **envp)
{
	int	i;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			dprintf(cmd.outfd, "%s\n", envp[i]);
		i++;
	}
	return (0);
}
