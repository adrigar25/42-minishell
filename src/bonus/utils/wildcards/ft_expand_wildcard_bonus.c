/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/24 17:24:36 by agarcia          ###   ########.fr       */
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

static int	expand_loop(DIR *dir, char **matches, int max_matches,
		char *base_pattern, char *dirpath)
{
	struct dirent	*entry;
	int				result;
	int				count;

	count = 0;
	entry = readdir(dir);
	while (entry && count < max_matches)
	{
		if ((entry->d_name[0] != '.' || (base_pattern
					&& base_pattern[0] == '.'))
			&& ft_match_pattern(base_pattern, entry->d_name))
		{
			matches[count] = build_path(dirpath, entry->d_name);
			if (!matches[count])
				result = -1;
			else
				result = count + 1;
			if (result == -1)
				return (count);
			count = result;
		}
		entry = readdir(dir);
	}
	return (count);
}

int	ft_expand_wildcard(const char *pattern, char **matches, int max_matches)
{
	DIR	*dir;
	int	count;

	char *dirpath;            // Cambiar de char ** a char *
	const char *base_pattern; // Cambiar de char * a const char *
	count = 0;
	if (!pattern)
		return (0);
	get_dir_and_pattern(pattern, &dirpath, &base_pattern); // Pasar direcciones
	dir = opendir(dirpath);
	if (!dir)
	{
		free(dirpath);
		return (0);
	}
	count = expand_loop(dir, matches, max_matches, (char *)base_pattern,
			dirpath);
	closedir(dir);
	free(dirpath);
	return (count);
}
