/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:29:30 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 13:04:34 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	print_int(int fd, int v)
{
	return (ft_putnbr_fd((long)v, fd));
}

static int	print_uint(int fd, unsigned int v)
{
	return (ft_putnbr_fd((long)v, fd));
}

/**
 * ENGLISH: Handles a single format specifier for ft_fprintf,
 * 			printing the corresponding argument.
 *
 * SPANISH: Maneja un solo especificador de formato para ft_fprintf,
 * 			imprimiendo el argumento correspondiente.
 *
 * @param fd   The file descriptor to write to. /
 *             El descriptor de archivo al que escribir.
 *
 * @param spec   The format specifier character (e.g., 'd', 's', 'c'). /
 *               El carácter del especificador de formato (por ejemplo,
 * 				'd', 's', 'c').
 *
 * @param ap   Pointer to the variable argument list. /
 *             Puntero a la lista de argumentos variables.
 *
 * @returns The number of characters printed for this specifier. /
 *          El número de caracteres impresos para este especificador.
 */
static int	handle_format(int fd, char spec, va_list *ap)
{
	char	*str;

	if (spec == 's')
	{
		str = va_arg(*ap, char *);
		if (!str)
			str = "(null)";
		ft_putstr_fd(str, fd);
		return ((int)strlen(str));
	}
	else if (spec == 'd' || spec == 'i')
		return (print_int(fd, va_arg(*ap, int)));
	else if (spec == 'u')
		return (print_uint(fd, va_arg(*ap, unsigned int)));
	else if (spec == 'c')
		return (write(fd, &(char){va_arg(*ap, int)}, 1));
	else if (spec == '%')
		return (write(fd, "%", 1));
	else
		return (write(fd, &spec, 1));
}

/**
 * ENGLISH: Writes formatted output to the specified file descriptor,
 * 			similar to printf.
 *
 * SPANISH: Escribe una salida formateada en el descriptor de archivo
 * 			especificado, igual que printf.
 *
 * @param fd   The file descriptor to write to. /
 *             El descriptor de archivo al que escribir.
 *
 * @param fmt   The format string containing the text and format specifiers. /
 *              La cadena de formato que contiene el texto y los especificadores
 *              de formato.
 *
 * @param ...   Additional arguments to be formatted and printed. /
 *              Argumentos adicionales que serán formateados e impresos.
 *
 * @returns The total number of characters written. /
 *          El número total de caracteres escritos.
 */
int	ft_fprintf(int fd, const char *fmt, ...)
{
	va_list		ap;
	int			written;
	const char	*p = fmt;

	written = 0;
	va_start(ap, fmt);
	while (*p)
	{
		if (*p == '%' && *(p + 1))
		{
			written += handle_format(fd, *(++p), &ap);
			p++;
		}
		else
		{
			write(fd, p, 1);
			written++;
			p++;
		}
	}
	va_end(ap);
	return (written);
}
