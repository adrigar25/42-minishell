/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 14:14:54 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Expands a wildcard pattern by finding matching files in the current
 *          directory and storing them in the provided matches array.
 *
 * SPANISH: Expande un patrón comodín buscando archivos coincidentes en el
 *          directorio actual y almacenándolos en el arreglo de coincidencias
 *          proporcionado.
 *
 * @param pattern      The wildcard pattern to match files against. /
 *                     El patrón comodín para coincidir con los archivos.
 *
 * @param matches      An array to store the matching file names. /
 *                     Un arreglo para almacenar los nombres de archivos
 *                     coincidentes.
 *
 * @param max_matches  The maximum number of matches to store in the array. /
 *                     El número máximo de coincidencias para almacenar en el
 *                     arreglo.
 *
 * @returns The number of matches found and stored in the array. /
 *          El número de coincidencias encontradas y almacenadas en el arreglo.
 */
int	ft_expand_wildcard(const char *pattern, char **matches, int max_matches)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;
	char			*dirpath;
	const char		*base_pattern;
	size_t			plen;
	const char		*slash = NULL;
	char			*tmp;

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
	count = 0;
	entry = readdir(dir);
	while (entry && count < max_matches)
	{
		if ((entry->d_name[0] != '.' || (base_pattern
					&& base_pattern[0] == '.'))
			&& ft_match_pattern(base_pattern, entry->d_name))
		{
			if (ft_strcmp(dirpath, ".") == 0)
				matches[count] = ft_strdup(entry->d_name);
			else
			{
				matches[count] = ft_strjoin(dirpath, "/");
				if (!matches[count])
				{
					closedir(dir);
					free(dirpath);
					return (count);
				}
				{
					tmp = matches[count];
					matches[count] = ft_strjoin(tmp, entry->d_name);
					free(tmp);
					if (!matches[count])
					{
						closedir(dir);
						free(dirpath);
						return (count);
					}
				}
			}
			if (!matches[count])
			{
				closedir(dir);
				free(dirpath);
				return (count);
			}
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	free(dirpath);
	return (count);
}
