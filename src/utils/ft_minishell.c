/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 16:39:50 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <readline/readline.h>

int	ft_minishell(char **envp)
{
	int		argc;
	char	**argv;
	char	*input;
	char	*prompt;

	ft_msg_start();
	while (1)
	{
		prompt = ft_strjoin(ft_strjoin("\033[90mminishell:(",
					ft_get_directory_path(prompt)), ")\033[0m> ");
		input = readline(prompt);
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			argc = ft_count_args(input);
			argv = ft_parse_input(input, argc);
			if (!argv)
				return (1);
			if (has_pipe(argv))
				ft_pipex((const char **)argv, envp);
			else
				ft_exec_cmd(argv, 0, 1, envp);
		}
		free(input);
	}
	clear_history();
	return (0);
}
