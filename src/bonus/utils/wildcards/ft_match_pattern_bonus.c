/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_match_pattern_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:46:11 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 01:22:55 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Matches a filename against a wildcard pattern.
 *          Supports '*' (matches any sequence of characters)
 *          and '?' (matches any single character).
 *
 * SPANISH: Compara un nombre de archivo con un patrón comodín.
 *          Soporta '*' (coincide con cualquier secuencia de caracteres)
 *          y '?' (coincide con cualquier carácter individual).
 *
 * @attention Hidden files (starting with '.') are not matched by wildcards. /
 *       Los archivos ocultos (que comienzan con '.') no son coincidentes con
 *       comodines.
 *
 * @param pattern   The wildcard pattern to match against. /
 *                  El patrón comodín para comparar.
 *
 * @param filename  The filename to be matched. /
 *                  El nombre de archivo a comparar.
 *
 * @returns 1 if the filename matches the pattern, 0 otherwise. /
 *          1 si el nombre de archivo coincide con el patrón, 0 en
 *          caso contrario.
 */
int	ft_match_pattern(const char *pattern, const char *filename)
{
	const char	*pat;
	const char	*str;
	const char	*last_star;
	const char	*str_backup;

	pat = pattern;
	str = filename;
	last_star = NULL;
	str_backup = NULL;
	while (*str)
	{
		if (*pat == '?' || *pat == *str)
		{
			pat++;
			str++;
		}
		else if (*pat == '*')
		{
			last_star = pat++;
			str_backup = str;
		}
		else if (last_star)
		{
			pat = last_star + 1;
			str = ++str_backup;
		}
		else
			return (0);
	}
	while (*pat == '*')
		pat++;
	return (*pat == '\0');
}
