/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_in_dir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:26:48 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:51:28 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*search_file_in_entries(DIR *d, const char *dir,
	const char *filename)
{
	struct dirent	*entry;
	char			*result;

	result = NULL;
	entry = readdir(d);
	while (entry)
	{
		if (ft_is_dot_or_dotdot(entry->d_name))
		{
			entry = readdir(d);
			continue ;
		}
		if (ft_strcmp(entry->d_name, filename) == 0)
		{
			result = ft_build_path(dir, entry->d_name);
			break ;
		}
		entry = readdir(d);
	}
	return (result);
}

char	*ft_search_in_dir(const char *dir, const char *filename)
{
	DIR		*d;
	char	*result;

	d = opendir(dir);
	result = NULL;
	if (!d)
		return (result);
	result = search_file_in_entries(d, dir, filename);
	closedir(d);
	if (!result)
		result = ft_search_in_subdirs(dir, filename);
	return (result);
}
