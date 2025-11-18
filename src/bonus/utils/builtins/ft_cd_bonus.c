/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/18 18:34:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Determines the target directory for the 'cd' command based
 * 			on the arguments and environment.
 *
 * SPANISH: Determina el directorio de destino para el comando 'cd' según
 * 			los argumentos y el entorno.
 *
 * @param argv  The argument vector; argv[1] may contain the target directory.
 *              El vector de argumentos; argv[1] puede contener el directorio
 * 				de destino.
 * @param envp  The environment variables.
 *              Las variables de entorno.
 *
 * @returns The target directory as a string, or NULL if not found.
 *          El directorio de destino como cadena, o NULL si no se encuentra.
 */
static char	*ft_get_target_dir(char **argv, char **envp)
{
	if (!argv[1])
		return (ft_getenv("HOME", envp));
	else if (ft_strcmp(argv[1], "~") == 0)
		return (ft_getenv("HOME", envp));
	else
		return (argv[1]);
}

/**
 * ENGLISH: Changes the current working directory to the target directory.
 *
 * SPANISH: Cambia el directorio de trabajo actual al directorio de destino.
 *
 * @param argv  The argument vector; argv[1] may contain the target directory.
 *              El vector de argumentos; argv[1] puede contener el
 * 				directorio de destino.
 * @param envp  A pointer to the environment variables array.
 *              Un puntero al array de variables de entorno.
 *
 * @returns 0 on success, 1 on failure.
 *          0 en caso de éxito, 1 en caso de error.
 */
int	ft_cd(char **argv, char ***envp)
{
	int		prev;
	char	*target_dir;

	target_dir = NULL;
	if (argv[1] && argv[2])
		return (ft_handle_error(7, EXIT_FAILURE, NULL, NULL));
	prev = (argv[1] && ft_strlen(argv[1]) == 1 && ft_strcmp(argv[1], "-") == 0);
	if (prev)
		target_dir = ft_getenv("OLDPWD", *envp);
	else
		target_dir = ft_get_target_dir(argv, *envp);
	if (!target_dir)
	{
		if (prev)
			return (ft_handle_error(11, EXIT_FAILURE, "OLDPWD", "not set"));
		return (ft_handle_error(8, EXIT_FAILURE, NULL, NULL));
	}
	if (chdir(target_dir) == 0)
	{
		if (prev)
			printf("%s\n", target_dir);
	}
	else
		return (ft_handle_error(11, EXIT_FAILURE, target_dir, strerror(errno)));
	return (ft_update_pwd_env(ft_getenv("PWD", *envp), target_dir, envp), 0);
}
