/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:51:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:58:33 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Appends the source string to the destination string,
 *          reallocating memory as necessary.
 *
 * SPANISH: Añade la cadena de origen a la cadena de destino,
 *          realojando memoria según sea necesario.
 *
 * @param dst   Pointer to the destination string to append to. /
 *              Puntero a la cadena de destino donde se añadirá.
 *
 * @param src   The source string to append. /
 *              La cadena de origen que se añadirá.
 *
 * @returns 1 on success, 0 on memory allocation failure. /
 *          1 en caso de éxito, 0 en caso de fallo de asignación de memoria.
 */
int	ft_append(char **dst, const char *src)
{
	char	*temp;

	if (!dst || !src)
		return (0);
	temp = *dst;
	*dst = ft_strjoin(temp, (char *)src);
	free(temp);
	if (!*dst)
		return (0);
	return (1);
}
