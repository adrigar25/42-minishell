/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:56:05 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/09 17:15:12 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*alloc_join(size_t len1, size_t len2)
{
	char	*p;

	p = malloc(len1 + len2 + 1);
	return (p);
}

static void	copy_and_advance(char *dst, const char *src, size_t *pos)
{
	size_t	i;

	if (!src)
		return ;
	i = 0;
	while (src[i])
		dst[(*pos)++] = src[i++];
}

/**
 * ENGLISH: Joins two strings into a new string.
 *
 * SPANISH: Une dos cadenas en una nueva cadena.
 *
 * @param s1  The first string. /
 *				La primera cadena.
 * @param s2  The second string. /
 *				La segunda cadena.
 *
 * @returns A pointer to the new string, or NULL on failure. /
 *				Un puntero a la nueva cadena, o NULL en caso de error.
 */
char	*ft_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	i;
	size_t	len1;
	size_t	len2;

	if (!s2)
		return (NULL);
	if (s1)
		len1 = ft_strlen(s1);
	else
		len1 = 0;
	len2 = ft_strlen(s2);
	new_str = alloc_join(len1, len2);
	if (!new_str)
		return (NULL);
	i = 0;
	copy_and_advance(new_str, s1, &i);
	copy_and_advance(new_str, s2, &i);
	new_str[i] = '\0';
	return (new_str);
}
