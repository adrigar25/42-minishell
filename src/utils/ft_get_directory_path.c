/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_home_directory_path.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:44:08 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 17:45:10 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *ft_get_directory_path(char *dest)
{
	size_t lens[3];
	char *dir[2];

	dir[0] = getenv("HOME");
	dir[1] = getenv("PWD");
	if (dir[1] && dir[0] && ft_strstr(dir[1], dir[0]) == dir[1])
	{
		dest = malloc(ft_strlen(dir[1]) - ft_strlen(dir[0]) + 2);
		if (dest)
		{
			dest[0] = '~';
			if (dir[1][ft_strlen(dir[0])] == '/')
				ft_strcpy(dest + 1, dir[1] + ft_strlen(dir[0]));
			else if (dir[1][ft_strlen(dir[0])] != '\0')
			{
				dest[1] = '/';
				ft_strcpy(dest + 2, dir[1] + ft_strlen(dir[0]));
			}
			else
				dest[1] = '\0';
		}
	}
	else if (dir[1])
		dest = dir[1];
	return (dest);
}
