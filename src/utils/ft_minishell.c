/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/08 01:00:37 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

int	ft_minishell(char **envp)
{
	char	**argv;
	int		argc;
	char	*input;
	char	*prompt;
	t_cmd	*cmd_list;
	t_cmd	*curr;
	int		i;

	ft_msg_start();
	ft_init_signals();
	while (1)
	{
		prompt = ft_generate_prompt();
		input = readline(prompt);
		free(prompt);
		if (!input) // Solo salir si es EOF (Ctrl+D)
			break ;
		if (!*input) // Si está vacío, continuar sin procesar
		{
			free(input);
			continue ;
		}
		add_history(input);
		argc = ft_count_args(input);
		argv = ft_split_input(input, argc);
		cmd_list = ft_parse_input(argv, argc);
		curr = cmd_list;
		i = 0;
		while (curr)
		{
			if (curr->argv && curr->argv[0])
			{
				printf("cmd%d: args:", i);
				for (int j = 0; curr->argv[j]; j++)
					printf(" %s", curr->argv[j]);
				printf(" fdin:%d fdout:%d\n", curr->infd, curr->outfd);
			}
			curr = curr->next;
			i++;
		}
	}
	return (0);
}
