/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 20:14:23 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>

static int	ft_count_args_from_array(char **argv)
{
	int	count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}

static void	ft_show_argv(char *title, char *prefix, char **argv, int argc)
{
	int	i;

	printf("%s:\n", title);
	i = 0;
	while (i < argc)
	{
		printf("  %s[%d]: %s\n", prefix, i, argv[i]);
		i++;
	}
}

static void	ft_show_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*curr;
	int		arg_count;

	curr = cmd_list;
	while (curr)
	{
		printf("cmd: \n");
		arg_count = 0;
		if (curr->argv)
			arg_count = ft_count_args_from_array(curr->argv);
		ft_show_argv("  argv", "argv", curr->argv, arg_count);
		printf("  fd_in: %d, fd_out: %d\n", curr->infd, curr->outfd);
		printf("  has_error: %d\n", curr->has_error);
		curr = curr->next;
		if (curr)
			printf("  |\n  v\n");
	}
}

void	ft_show_debug(char **argv, int argc, char **expanded_argv,
		t_cmd *cmd_list)
{
	int	i;

	printf("\033[38;5;208m========== DEBUG INFO ==========\033[0m\n");
	if (argv)
	{
		printf("\033[34mArgumentos recibidos:\033[0m\n");
		i = 0;
		while (i < argc)
		{
			printf("  argv[%d]: %s\n", i, argv[i]);
			i++;
		}
	}
	if (expanded_argv)
	{
		printf("\n\033[34mArgumentos después de expandir:\033[0m\n");
		ft_show_argv("Expanded argv", "expanded_argv", expanded_argv, argc);
	}
	if (cmd_list)
	{
		printf("\n\033[34mComandos parseados:\033[0m\n");
		ft_show_cmd_list(cmd_list);
	}
	printf("\033[38;5;208m================================\033[0m\n\n");
}
