/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:30:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 16:36:12 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Gets the full path of a command by searching in the PATH
 * 			environment variable.
 *
 * SPANISH: Obtiene la ruta completa de un comando buscando en la variable
 * 			de entorno PATH.
 *
 * @param cmd   The command to search for. /
 *              El comando a buscar.
 *
 * @returns The full path to the command if found, or NULL otherwise. /
 *          La ruta completa al comando si se encuentra, o NULL en caso
 *           contrario.
 */
char	*ft_get_cmd_path(char *cmd)
{
	char	*temp;
	char	**paths;
	char	*path;
	int		i;
	int		j;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = ft_split(getenv("PATH"), ':');
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, X_OK) == 0)
		{
			ft_free_matrix(paths);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_matrix(paths);
	return (NULL);
}
