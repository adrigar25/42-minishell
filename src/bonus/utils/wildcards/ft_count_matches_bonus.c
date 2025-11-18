/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_matches_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:19 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 01:38:23 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static const char	*find_last_slash(const char *pattern)
{
	size_t	plen;

	plen = ft_strlen(pattern);
	while (plen > 0)
	{
		if (pattern[plen - 1] == '/')
			return (pattern + (plen - 1));
		plen--;
	}
	return (NULL);
}

static void	get_dir_and_pattern(const char *pattern, char **dirpath,
		const char **base_pattern)
{
	const char	*slash;

	slash = find_last_slash(pattern);
	if (slash)
	{
		*dirpath = ft_substr((char *)pattern, 0, (size_t)(slash - pattern));
		*base_pattern = slash + 1;
	}
	else
	{
		*dirpath = ft_strdup(".");
		*base_pattern = pattern;
	}
}

static int	count_dir_matches(DIR *dir, const char *base_pattern)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if ((entry->d_name[0] != '.' || (base_pattern
					&& base_pattern[0] == '.'))
			&& ft_match_pattern(base_pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	return (count);
}

int	ft_count_matches(const char *pattern)
{
	DIR			*dir;
	int			count;
	char		*dirpath;
	const char	*base_pattern;

	if (!pattern)
		return (0);
	get_dir_and_pattern(pattern, &dirpath, &base_pattern);
	dir = opendir(dirpath);
	if (!dir)
	{
		free(dirpath);
		return (0);
	}
	count = count_dir_matches(dir, base_pattern);
	closedir(dir);
	free(dirpath);
	return (count);
}
