/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:35:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 12:54:43 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * ENGLISH: Compares up to n characters of the null-terminated strings s1
 *          and s2.
 *
 * SPANISH: Compara hasta n caracteres de las cadenas terminadas en nulo s1
 *          y s2.
 *
 * @param s1   The first string to compare. /
 *              La primera cadena a comparar.
 *
 * @param s2   The second string to compare. /
 *              La segunda cadena a comparar.
 *
 * @param n    The maximum number of characters to compare. /
 *              El número máximo de caracteres a comparar.
 *
 * @returns An integer less than, equal to, or greater than zero if s1 (or the
 *          first n bytes thereof) is found, respectively, to be less than,
 *          to match, or be greater than s2. /
 *          Un entero menor que, igual a, o mayor que cero si s1 (o los primeros
 *          n bytes de la misma) es encontrado, respectivamente, como menor que,
 *          igual a, o mayor que s2.
 * @returns The comparison is done using unsigned characters. /
 *          La comparación se hace usando caracteres sin signo.
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	if (i == n)
		return (0);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
