/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:41:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/10 16:49:52 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
 * ENGLISH: Outputs a string to the standard error output.
 *
 * SPANISH: Envía una cadena a la salida de error estándar.
 *
 * @param str   The string to output. /
 *              La cadena a enviar.
 *
 * @returns 0 on success, -1 on failure. /
 *          0 en caso de éxito, -1 en caso de error.
 */
int	ft_putstr_error(const char *str)
{
	if (!str)
	{
		if (ft_putstr_error("Error: NULL string\n") == -1)
			return (-1);
		return (0);
	}
	while (*str)
	{
		if (ft_putchar_error(*str) == -1)
			return (-1);
		str++;
	}
	return (0);
}
