/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 18:10:54 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
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

void	ft_show_debug(char **argv, int argc, char **expanded_argv,
		t_cmd *cmd_list)
{
	t_cmd	*curr;
	int		i;
	int		arg_count;

	printf("=== DEBUG INFO ===\n");
	if (argv)
	{
		printf("Argumentos recibidos:\n");
		i = 0;
		while (i < argc)
		{
			printf("  argv[%d]: %s\n", i, argv[i]);
			i++;
		}
	}
	if (expanded_argv)
	{
		printf("\nArgumentos después de expandir:\n");
		ft_show_argv("Expanded argv", "expanded_argv", expanded_argv, argc);
	}
	if (cmd_list)
	{
		printf("\nComandos parseados:\n");
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
	printf("==================\n\n");
}
