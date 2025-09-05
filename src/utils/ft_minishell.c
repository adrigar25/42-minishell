/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 17:15:48 by agarcia          ###   ########.fr       */
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
	pid_t	pid;
	int		status;

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
			for (int i = 0; argv[i]; i++)
				printf("argv[%d]: %s\n", i, argv[i]);
			if (!argv)
				return (1);
			if (has_pipe(argv))
				ft_pipex((const char **)argv, envp);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					ft_exec_cmd(argv, 0, 1, envp);
					exit(EXIT_SUCCESS);
				}
				else if (pid > 0)
					waitpid(pid, &status, 0);
			}
		}
		free(input);
	}
	clear_history();
	return (0);
}
