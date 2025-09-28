/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_numeric.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:38:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/28 16:56:48 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * ENGLISH: Checks if a string represents a valid numeric value,
 *          including optional leading '+' or '-' signs and ensuring
 *          the value fits within the range of a long long integer.
 *
 * SPANISH: Verifica si una cadena representa un valor numérico válido,
 *          incluyendo signos '+' o '-' opcionales al principio y asegurando
 *          que el valor se ajuste al rango de un entero largo largo.
 *
 * @param str   The string to check. /
 *              La cadena a verificar.
 *
 * @returns 1 if the string is a valid numeric value, 0 otherwise. /
 *          1 si la cadena es un valor numérico válido, 0 en caso contrario.
 */
static int	ft_is_valid_sign(const char *str, int *sign, int *i)
{
	*sign = 1;
	*i = 0;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			*sign = -1;
		(*i)++;
	}
	if (str[*i] == '\0')
		return (0);
	return (1);
}

/**
 * ENGLISH: Helper function to check the numeric body of the string.
 * 		It ensures all characters are digits and checks for overflow.
 *
 * SPANISH: Función auxiliar para verificar el cuerpo numérico de la cadena.
 * 		Asegura que todos los caracteres sean dígitos y verifica el
 * 		desbordamiento
 *
 * @param str   The string to check. /
 * 			La cadena a verificar.
 * @param i     The starting index to check from. /
 * 			El índice de inicio desde el cual verificar.
 * @param sign  The sign of the number (1 for positive, -1 for negative
 * 			El signo del número (1 para positivo, -1 para negativo).
 *
 * @returns 1 if the numeric body is valid, 0 otherwise. /
 * 			1 si el cuerpo numérico es válido, 0 en caso contrario.
 */
static int	ft_is_numeric_body(const char *str, int i, int sign)
{
	long long			result;
	unsigned long long	limit;

	result = 0;
	if (sign == 1)
		limit = LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if ((unsigned long long)result > (limit - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

/**
 * ENGLISH: Checks if a string represents a valid numeric value,
 * 		including optional leading '+' or '-' signs and ensuring
 * 		the value fits within the range of a long long integer.
 *
 * SPANISH: Verifica si una cadena representa un valor numérico válido,
 * 		incluyendo signos '+' o '-' opcionales al principio y asegurando
 * 		que el valor se ajuste al rango de un entero largo largo.
 *
 * @param str   The string to check. /
 * 			La cadena a verificar.
 *
 * @returns 1 if the string is a valid numeric value, 0 otherwise. /
 * 			1 si la cadena es un valor numérico válido, 0 en caso contrario
 */
int	ft_is_numeric(const char *str)
{
	int	i;
	int	sign;

	if (!str || *str == '\0')
		return (0);
	if (!ft_is_valid_sign(str, &sign, &i))
		return (0);
	return (ft_is_numeric_body(str, i, sign));
}
