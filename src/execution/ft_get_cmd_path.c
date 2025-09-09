/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_cmd_path.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 16:30:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 17:15:24 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_char_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

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
			free_char_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_char_array(paths);
	return (NULL);
}
