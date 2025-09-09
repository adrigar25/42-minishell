/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 19:54:29 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_putarg_echo(char *arg, int flag_n, int outfd)
{
	int	i;
	int	len;
	int	quote = 0;

	if (!arg)
	{
		if (!flag_n)
			ft_putchar_fd('\n', outfd);
		return (0);
	}
	len = ft_strlen(arg);
	if ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\'' && arg[len - 1] == '\''))
	{
		quote = 1;
		i = 1;
		len--;
	}
	else
		i = 0;
	while (i < len)
	{
		if (arg[i] == '\\' && arg[i + 1] == 'n')
		{
			ft_putchar_fd('\\', outfd);
			ft_putchar_fd('n', outfd);
			i += 2;
			continue ;
		}
		ft_putchar_fd(arg[i], outfd);
		i++;
	}
	if (flag_n)
	{
		if (arg[len - 1] == ' ')
			ft_putchar_fd('%', outfd);
	}
	return (0);
}

static int	ft_intflag_n(char *arg)
{
	if (arg && arg[0] == '-' && arg[1] == 'n')
		return (1);
	return (0);
}

int	ft_echo(t_cmd cmd)
{
	int	i;
	int	n_flag;
	int	newline;
	int	outfd;

	n_flag = ft_intflag_n(cmd.argv[1]);
	ft_putarg_echo(cmd.argv[1 + n_flag], n_flag, cmd.outfd);
	return (0);
}
