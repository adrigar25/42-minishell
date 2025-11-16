/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_escaped.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 20:29:49 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/16 22:38:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

/*
** Helper: cuenta cuántas barras inversas ('\\') consecutivas hay justo
** antes de la posición idx en la cadena s.
** Ej: s = "abc\\\\def", idx apunta a 'd' -> devuelve 2
*/

/*
** ft_is_escaped:
** - Devuelve 1 si el carácter en s[pos] está escapado por un número impar
**   de '\\' inmediatamente antes de pos.
** - Tiene en cuenta comillas: dentro de comillas simples (') nada se
**   interpreta como escape; dentro de comillas dobles (") un quote puede
**   estar escapado por '\\'. La función recorre la cadena desde el inicio
**   hasta pos para determinar si, en ese punto, estamos dentro de comillas.
*/
int	ft_is_escaped(const char *s, int idx)
{
	int	count;

	count = 0;
	idx--;
	while (idx >= 0 && s[idx] == '\\')
	{
		count++;
		idx--;
	}
	return (count % 2 == 1);
}
