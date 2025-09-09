/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 19:01:35 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_putarg_echo(char *arg, int flag_n, int outfd)
{
	int	i;

	printf("entro");
	i = 0;
	while (arg[i] != '\0')
	{
		ft_putchar_fd(arg[i], outfd);
		i++;
	}
	if (arg[i - 1] != '\n' && !flag_n)
		ft_putchar_fd('\n', outfd);
	else if (arg[i - 1] == '\n' && flag_n)
		ft_putchar_fd('%', outfd);
	return (0);
}

int	ft_intflag_n(char *arg)
{
	int	i;

	printf("arg: %s\n", arg);
	if (arg[0] == '-')
		return (0);
	return (1);
}

int	ft_echo(t_cmd cmd)
{
	int	i;
	int	n_flag;
	int	newline;
	int	outfd;

	printf("argv[0]: %s\n", cmd.argv[0]);
	n_flag = ft_intflag_n(cmd.argv[1]);
	printf("n_flag: %d\n", n_flag);
	ft_putarg_echo(cmd.argv[1 + n_flag], n_flag, cmd.outfd);
	return (0);
}
