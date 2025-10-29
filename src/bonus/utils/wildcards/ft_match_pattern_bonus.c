/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_match_pattern_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:46:11 by agarcia           #+#    #+#             */
/*   Updated: 2025/10/29 17:05:45 by adriescr         ###   ########.fr       */
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
	const char	*p = pattern;
	const char	*s = filename;
	const char	*star = NULL;
	const char	*ss = NULL;

	while (*s)
	{
		if (*p == '?' || *p == *s)
		{
			p++;
			s++;
		}
		else if (*p == '*')
		{
			star = p++;
			ss = s;
		}
		else if (star)
		{
			p = star + 1;
			s = ++ss;
		}
		else
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}
