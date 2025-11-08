/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/06 17:41:51 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Implements the pwd command, printing the current working directory
 * 			to the specified output file descriptor.
 *
 * SPANISH: Implementa el comando pwd, imprimiendo el directorio de trabajo
 * 			actual en el descriptor de archivo de salida especificado.
 *
 * @param cmd The command structure containing file descriptors. /
 *            La estructura de comando que contiene descriptores de archivo.
 *
 * @returns 0 on success, 1 on error. /
 *          0 en caso de éxito, 1 en caso de error.
 */
int	ft_pwd(t_cmd cmd)
{
	char	cwd[1024];
	char	*output;
	int		len;
	char	*pwd_env;

	pwd_env = NULL;
	if (cmd.data && cmd.data->envp)
		pwd_env = ft_getenv("PWD", cmd.data->envp);
	if (pwd_env && pwd_env[0] == '/')
	{
		len = ft_strlen(pwd_env);
		output = malloc(len + 2);
		if (!output)
			return (1);
		ft_strcpy(output, pwd_env);
		output[len] = '\n';
		output[len + 1] = '\0';
		write(cmd.outfd, output, len + 1);
		free(output);
		return (0);
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		len = ft_strlen(cwd);
		output = malloc(len + 2);
		if (!output)
			return (1);
		ft_strcpy(output, cwd);
		output[len] = '\n';
		output[len + 1] = '\0';
		write(cmd.outfd, output, len + 1);
		free(output);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
