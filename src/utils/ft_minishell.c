/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/08 16:01:45 by adriescr         ###   ########.fr       */
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
	}
	return (0);
}
