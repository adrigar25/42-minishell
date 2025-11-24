/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/24 18:21:20 by agarcia          ###   ########.fr       */
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

static int	expand_loop(DIR *dir, char **matches, int max_matches,
		char *base_pattern)
{
	struct dirent	*entry;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry && count < max_matches)
	{
		if ((entry->d_name[0] != '.' || (base_pattern
					&& base_pattern[0] == '.'))
			&& ft_match_pattern(base_pattern, entry->d_name))
		{
			matches[count] = ft_strdup((char *)entry->d_name);
			if (!matches[count])
				return (count);
			count++;
		}
		entry = readdir(dir);
	}
	return (count);
}

static int	build_full_paths(char **matches, int count, const char *dirpath)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (i < count)
	{
		if (ft_strcmp(dirpath, ".") == 0)
			full_path = ft_strdup((char *)matches[i]);
		else
		{
			tmp = ft_strjoin((char *)dirpath, "/");
			if (!tmp)
				return (-1);
			full_path = ft_strjoin(tmp, (char *)matches[i]);
			free(tmp);
		}
		if (!full_path)
			return (-1);
		free(matches[i]);
		matches[i] = full_path;
		i++;
	}
	return (0);
}

int	ft_expand_wildcard(const char *pattern, char **matches, int max_matches)
{
	DIR			*dir;
	int			count;
	char		*dirpath;
	const char	*base_pattern;

	count = 0;
	if (!pattern)
		return (0);
	get_dir_and_pattern(pattern, &dirpath, &base_pattern);
	dir = opendir(dirpath);
	if (!dir)
	{
		free(dirpath);
		return (0);
	}
	count = expand_loop(dir, matches, max_matches, (char *)base_pattern);
	closedir(dir);
	if (count > 0)
		build_full_paths(matches, count, dirpath);
	free(dirpath);
	return (count);
}
