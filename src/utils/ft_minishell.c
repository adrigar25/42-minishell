/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/08 19:45:08 by agarcia          ###   ########.fr       */
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
	pid_t	pid;
	int		status;

	ft_msg_start();
	ft_init_signals();
	while (1)
	{
		prompt = ft_generate_prompt();
		input = readline(prompt);
		free(prompt);
		if (!input)
			break ;
		if (!*input)
		{
			free(input);
			continue ;
		}
		add_history(input);
		argc = ft_count_args(input);
		printf("argc: %d\n", argc);
		argv = ft_split_input(input, argc);
		printf("argv:\n");
		i = 0;
		while (i < argc)
		{
			printf("  argv[%d]: '%s'\n", i, argv[i]);
			i++;
		}
		cmd_list = ft_parse_input(argv, argc);
		curr = cmd_list;
		i = 0;
		while (curr)
		{
			printf("cmd: %d\n", i);
			printf("  args:\n");
			for (int j = 0; curr->argv && curr->argv[j]; j++)
				printf("    [%d]: '%s'\n", j, curr->argv[j]);
			printf("  fds:\n");
			printf("    infds:\n");
			for (int j = 0; curr->infds && curr->infds[j] != -1; j++)
				printf("      [%d]: %d\n", j, curr->infds[j]);
			printf("    outfds:\n");
			for (int j = 0; curr->outfds && curr->outfds[j] != -1; j++)
				printf("      [%d]: %d\n", j, curr->outfds[j]);
			curr = curr->next;
			i++;
		}
	}
	return (0);
}
