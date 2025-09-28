/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_matches_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:42:19 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:21:30 by adriescr         ###   ########.fr       */
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

	count = 0;
	dir = opendir(".");
	if (!dir)
		return (0);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] != '.' && ft_match_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}
