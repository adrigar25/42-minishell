/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_wildcard_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:33:12 by adriescr         ###   ########.fr       */
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

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry && count < max_matches)
	{
		if (entry->d_name[0] != '.' && ft_match_pattern(pattern, entry->d_name))
		{
			matches[count] = ft_strdup(entry->d_name);
			if (!matches[count])
			{
				closedir(dir);
				return (count);
			}
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}
