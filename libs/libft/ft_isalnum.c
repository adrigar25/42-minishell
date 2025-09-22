/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 12:53:17 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * ENGLISH: Counts the number of words in a string,
 *          separated by a given delimiter.
 *
 * SPANISH: Cuenta el número de palabras en una cadena,
 *          separadas por un delimitador dado.
 *
 * @param s   The string to count words in. /
 *            La cadena en la que contar las palabras.
 *
 * @param c   The delimiter character. /
 *            El carácter delimitador.
 *
 * @returns The number of words in the string. /
 *          El número de palabras en la cadena.
 */
int	ft_isalnum(int c)
{
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a'
			&& c <= 'z'));
}
