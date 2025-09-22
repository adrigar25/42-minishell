/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_pwd_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:00:34 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:34:47 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Updates the PWD and OLDPWD environment variables.
 *          If getcwd fails, it sets PWD to the provided target_dir.
 *
 * SPANISH: Actualiza las variables de entorno PWD y OLDPWD.
 *          Si getcwd falla, establece PWD al directorio objetivo proporcionado.
 *
 * @param oldpwd      The previous working directory to set as OLDPWD.
 *                    Can be NULL. /
 *                    El directorio de trabajo anterior para establecer
 *	 					como OLDPWD. Puede ser NULL.
 *
 * @param target_dir  The target directory to set as PWD if getcwd fails. /
 *                    El directorio objetivo para establecer como PWD si
 * 					getcwd falla.
 *
 * @param envp        Pointer to the environment variable array. /
 *                    Puntero al array de variables de entorno.
 */
void	ft_update_pwd_env(char *oldpwd, char *target_dir, char ***envp)
{
	char	buf[4096];

	if (getcwd(buf, sizeof(buf)))
	{
		if (oldpwd)
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", buf, envp);
	}
	else
	{
		if (oldpwd)
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", target_dir, envp);
	}
}
