/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_matches_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:19 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/13 17:39:52 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Counts the number of files in the current directory
 *          that match the given wildcard pattern.
 *
 * SPANISH: Cuenta el número de archivos en el directorio actual
 *          que coinciden con el patrón comodín dado.
 *
 * @param pattern   The wildcard pattern to match files against. /
 *                  El patrón comodín para coincidir con los archivos.
 *
 * @returns The number of matching files. /
 *          El número de archivos que coinciden.
 */
int	ft_count_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	char			*dirpath;
	const char		*base_pattern;
	size_t			plen;
	const char		*slash = NULL;

	count = 0;
	if (!pattern)
		return (0);
	{
		plen = ft_strlen(pattern);
		while (plen > 0)
		{
			if (pattern[plen - 1] == '/')
			{
				slash = pattern + (plen - 1);
				break ;
			}
			plen--;
		}
		if (slash)
		{
			dirpath = ft_substr((char *)pattern, 0, (size_t)(slash - pattern));
			base_pattern = slash + 1;
		}
		else
		{
			dirpath = ft_strdup(".");
			base_pattern = pattern;
		}
	}
	dir = opendir(dirpath);
	if (!dir)
	{
		free(dirpath);
		return (0);
	}
	entry = readdir(dir);
	while (entry != NULL)
	{
		if ((entry->d_name[0] != '.' || (base_pattern
					&& base_pattern[0] == '.'))
			&& ft_match_pattern(base_pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	free(dirpath);
	return (count);
}
