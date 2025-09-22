/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:01:01 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 14:42:14 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * ENGLISH: Counts the number of environment variables in a NULL-terminated array.
 *
 * SPANISH: Cuenta el número de variables de entorno en un arreglo terminado en NULL.
 *
 * @param envp   The NULL-terminated array of environment variable strings. /
 *               El arreglo de cadenas de variables de entorno terminado en NULL.
 *
 * @returns The number of environment variables in the array. /
 *          El número de variables de entorno en el arreglo.
 */
int	ft_count_arg(char **args)
{
	int	count;

	count = 0;
	while (args && args[count])
		count++;
	return (count);
}
