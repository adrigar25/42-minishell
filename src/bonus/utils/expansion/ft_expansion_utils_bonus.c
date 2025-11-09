/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/08 02:13:04 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Copies a literal substring from the argument string to the
 *          destination string.
 *
 * SPANISH: Copia una subcadena literal de la cadena de argumentos a la
 *          cadena de destino.
 *
 * @param dst   Pointer to the destination string to append to. /
 *              Puntero a la cadena de destino donde se añadirá.
 *
 * @param arg   The argument string containing the literal substring. /
 *              La cadena de argumentos que contiene la subcadena literal.
 *
 * @param start The starting index of the substring in the argument string. /
 *              El índice inicial de la subcadena en la cadena de argumentos.
 *
 * @param end   The ending index (exclusive) of the substring in the
 *              argument string. /
 *              El índice final (exclusivo) de la subcadena en la cadena
 *              de argumentos.
 *
 * @returns 1 on success, 0 on memory allocation failure. /
 *          1 en caso de éxito, 0 en caso de fallo de asignación de memoria.
 */
int	ft_copy_literal(char **dst, char *arg, int start, int end)
{
	char	*literal;
	char	*temp;

	if (end <= start)
		return (1);
	literal = ft_substr(arg, start, end - start);
	if (!literal)
		return (0);
	temp = *dst;
	*dst = ft_strjoin(temp, literal);
	free(temp);
	free(literal);
	if (!*dst)
		return (0);
	return (1);
}
