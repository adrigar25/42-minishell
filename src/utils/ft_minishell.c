/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/06 00:58:01 by agarcia          ###   ########.fr       */
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
	int		fd_in;
	pid_t	pid;
	int		status;
	char	**cmd_argv;

	ft_msg_start();
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	prompt = NULL;
	fd_in = 0;
	while (1)
	{
		char *dir_path = ft_get_directory_path(NULL);
		char *temp = ft_strjoin("\033[90mminishell:(", dir_path);
		if (prompt)
			free(prompt);
		prompt = ft_strjoin(temp, ")\033[0m> ");
		free(temp);
		free(dir_path);
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
			fd_in = 0;
			cmd_argv = argv;
			if (argv[0] && ft_strstr(argv[0], "<<") != NULL)
			{
				if (!argv[1])
				{
					printf("Error: missing delimiter for heredoc\n");
					continue ;
				}
				fd_in = ft_handle_here_doc(argv[1]);
				if (fd_in == -1)
				{
					printf("Error processing heredoc\n");
					continue ;
				}
				cmd_argv = &argv[2];
			}
			if (has_pipe(cmd_argv))
			{
				ft_pipex((const char **)cmd_argv, fd_in, envp);
			}
			else
			{
				// Manejar builtins en el proceso padre
				if (ft_handle_builtins(cmd_argv, &envp))
				{
					// Se ejecutó un builtin, no hacer fork
					if (fd_in != 0)
						close(fd_in);
				}
				else
				{
					// No es builtin, hacer fork para comando externo
					pid = fork();
					if (pid == 0)
					{
						signal(SIGINT, SIG_DFL);
						signal(SIGQUIT, SIG_DFL);
						ft_exec_cmd(cmd_argv, fd_in, 1, envp);
						exit(EXIT_SUCCESS);
					}
					else if (pid > 0)
					{
						waitpid(pid, &status, 0);
						if (fd_in != 0)
							close(fd_in);
					}
				}
			}
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
