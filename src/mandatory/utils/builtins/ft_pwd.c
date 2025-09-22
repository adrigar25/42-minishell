/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 16:30:08 by adriescr         ###   ########.fr       */
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
