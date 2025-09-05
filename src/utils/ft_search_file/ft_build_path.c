/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:35:06 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/05 13:50:59 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_build_path(const char *dir, const char *entry)
{
	size_t	dir_len;
	size_t	entry_len;
	size_t	total_len;
	char	*path;

	dir_len = ft_strlen(dir);
	entry_len = ft_strlen(entry);
	total_len = dir_len + 1 + entry_len + 1;
	path = malloc(total_len);
	if (!path)
		return (NULL);
	ft_memcpy(path, dir, dir_len);
	path[dir_len] = '/';
	ft_memcpy(path + dir_len + 1, entry, entry_len);
	path[dir_len + 1 + entry_len] = '\0';
	return (path);
}
