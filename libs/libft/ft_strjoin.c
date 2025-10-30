/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:56:05 by adriescr          #+#    #+#             */
/*   Updated: 2025/10/30 01:43:46 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
	size_t	j;
	size_t	len1;
	size_t	len2;

	if (!s2)
		return (NULL);
	if (s1)
		len1 = ft_strlen(s1);
	else
		len1 = 0;
	len2 = ft_strlen(s2);
	new_str = malloc(len1 + len2 + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	if (s1)
	{
		while (i < len1)
		{
			new_str[i] = s1[i];
			i++;
		}
	}
	j = 0;
	while (j < len2)
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	return (new_str);
}
