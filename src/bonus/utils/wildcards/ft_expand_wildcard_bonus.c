/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 01:33:36 by agarcia          ###   ########.fr       */
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

static char	*build_path(const char *dirpath, const char *name)
{
	char	*tmp;
	char	*result;

	if (ft_strcmp(dirpath, ".") == 0)
		return (ft_strdup((char *)name));
	tmp = ft_strjoin((char *)dirpath, "/");
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, (char *)name);
	free(tmp);
	return (result);
}

static int	expand_loop(DIR *dir, t_expand_data *data)
{
	struct dirent	*entry;
	int				result;

	data->count = 0;
	entry = readdir(dir);
	while (entry && data->count < data->max_matches)
	{
		if ((entry->d_name[0] != '.' || (data->base_pattern
					&& data->base_pattern[0] == '.'))
			&& ft_match_pattern(data->base_pattern, entry->d_name))
		{
			data->matches[data->count] = build_path(data->dirpath,
					entry->d_name);
			if (!data->matches[data->count])
				result = -1;
			else
				result = data->count + 1;
			if (result == -1)
				return (data->count);
			data->count = result;
		}
		entry = readdir(dir);
	}
	return (data->count);
}

int	ft_expand_wildcard(const char *pattern, char **matches, int max_matches)
{
	DIR				*dir;
	t_expand_data	data;

	if (!pattern)
		return (0);
	get_dir_and_pattern(pattern, (char **)&data.dirpath, &data.base_pattern);
	dir = opendir(data.dirpath);
	if (!dir)
	{
		free((char *)data.dirpath);
		return (0);
	}
	data.matches = matches;
	data.max_matches = max_matches;
	data.count = expand_loop(dir, &data);
	closedir(dir);
	free((char *)data.dirpath);
	return (data.count);
}
