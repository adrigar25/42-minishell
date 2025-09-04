/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:56:05 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:56:06 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * ENGLISH: Joins two strings into a new string.
 *
 * SPANISH: Une dos cadenas en una nueva cadena.
 *
 * @param s1  The first string. /
 *			  La primera cadena.
 * @param s2  The second string. /
 *			  La segunda cadena.
 *
 * @returns A pointer to the new string, or NULL on failure. /
 *		 	Un puntero a la nueva cadena, o NULL en caso de error.
 */
char	*ft_strjoin(char *s1, char *s2)
{
	char	*new_str;
	size_t	i;
	size_t	j;

	if (!s1)
		s1 = ft_strdup("");
	if (!s2)
		return (NULL);
	new_str = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		new_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		new_str[i++] = s2[j++];
	new_str[i] = '\0';
	return (new_str);
}
