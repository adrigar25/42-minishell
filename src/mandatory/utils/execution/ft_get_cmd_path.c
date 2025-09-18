/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:30:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 14:06:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*get_cmd_path(char *cmd)
{
	char	*temp;
	char	**paths;
	char	*path;
	int		i;
	int		j;

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
