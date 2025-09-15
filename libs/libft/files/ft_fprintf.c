/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:29:30 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/15 20:30:45 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

static int	putnbr_fd(long n, int fd)
{
	char	buf[32];
	int		i;
	int		count;

	i = 0;
	count = 0;
	if (n == 0)
		return (write(fd, "0", 1));
	if (n < 0)
	{
		count += write(fd, "-", 1);
		n = -n;
	}
	while (n > 0)
	{
		buf[i++] = '0' + (n % 10);
		n /= 10;
	}
	while (i-- > 0)
		count += write(fd, &buf[i], 1);
	return (count);
}

static int	print_int(int fd, int v)
{
	return (putnbr_fd((long)v, fd));
}

static int	print_uint(int fd, unsigned int v)
{
	return (putnbr_fd((long)v, fd));
}

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
