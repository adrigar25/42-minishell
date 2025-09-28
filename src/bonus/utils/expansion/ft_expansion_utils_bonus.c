/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:11:23 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Copies a literal substring from the argument to the destination,
 * 		reallocating memory as needed.
 *
 * SPANISH: Copia una subcadena literal del argumento a la
 * 		cadena de destino, realojando memoria según sea necesario.
 *
 * @param dst   Pointer to the destination string to append to. /
 * 			Puntero a la cadena de destino a la que se añadirá.
 * @param arg   The argument string containing the literal substring. /
 * 			La cadena de argumentos que contiene la subcadena literal.
 * @param start The starting index of the literal substring in arg. /
 * 			El índice de inicio de la subcadena literal en arg.
 * @param end   The ending index (exclusive) of the literal substring in arg. /
 * 			El índice de fin (exclusivo) de la subcadena literal en arg
 *
 * @returns 1 on success, 0 on memory allocation failure. /
 * 			1 en caso de éxito, 0 en caso de fallo de asignación de memoria.
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
