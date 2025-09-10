/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 10:39:18 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>

static void	ft_free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		if (cmd_list->argv)
		{
			i = 0;
			while (cmd_list->argv[i])
			{
				free(cmd_list->argv[i]);
				i++;
			}
			free(cmd_list->argv);
		}
		free(cmd_list);
		cmd_list = tmp;
	}
}

int	ft_minishell(char **envp)
{
	char	*input;
	int		argc;
	char	**argv;
	char	**expanded_argv;
	char	*prompt;
	t_cmd	*cmd_list;
	pid_t	pid;
	char	**envp_cpy;
	t_cmd	*curr;
	int		i;

	ft_save_envp(&envp_cpy, envp);
	ft_msg_start();
	ft_init_signals();
	while (1)
	{
		cmd_list = NULL;
		prompt = ft_generate_prompt(envp_cpy);
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
		argv = ft_split_input(input, argc);
		printf("Argumentos recibidos:\n");
		for (i = 0; i < argc; i++)
			printf("argv[%d]: %s\n", i, argv[i]);
		expanded_argv = ft_handle_env_expansion(argc, argv, envp_cpy);
		if (!expanded_argv)
			expanded_argv = argv;
		printf("Argumentos después de expandir:\n");
		for (i = 0; i < argc; i++)
			printf("argv[%d]: %s\n", i, expanded_argv[i]);
		free(input);
		cmd_list = ft_parse_input(expanded_argv, argc);
		// Liberar argv original
		while (argc-- > 0)
			free(argv[argc]);
		free(argv);
		// Liberar expanded_argv si es diferente de argv
		if (expanded_argv != argv)
		{
			i = 0;
			while (expanded_argv[i])
			{
				free(expanded_argv[i]);
				i++;
			}
			free(expanded_argv);
		}
		if (!cmd_list)
			continue ;
		printf("Comandos parseados:\n");
		curr = cmd_list;
		while (curr)
		{
			printf("cmd: \n");
			for (i = 0; curr->argv && curr->argv[i]; i++)
				printf("argv[%d]: %s\n", i, curr->argv[i]);
			printf("\nfd_in: %d, fd_out: %d\n", curr->infd, curr->outfd);
			curr = curr->next;
		}
		if (cmd_list && cmd_list->argv && strcmp(cmd_list->argv[0],
				"exit") == 0)
		{
			ft_free_cmd_list(cmd_list);
			break ;
		}
		curr = cmd_list;
		while (curr)
		{
			// Ejecutar builtins en el proceso padre
			if (curr->argv[0] && (ft_strcmp(curr->argv[0], "cd") == 0
					|| ft_strcmp(curr->argv[0], "export") == 0
					|| ft_strcmp(curr->argv[0], "unset") == 0))
			{
				ft_handle_builtins(curr, &envp_cpy);
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					if (ft_exec_cmd(curr, &envp_cpy) == -1)
					{
						ft_free_cmd_list(cmd_list);
						exit(EXIT_FAILURE);
					}
					exit(EXIT_SUCCESS);
				}
				else if (pid > 0)
				{
					if (curr->infd != STDIN_FILENO)
						close(curr->infd);
					if (curr->outfd != STDOUT_FILENO)
						close(curr->outfd);
					waitpid(pid, NULL, 0);
				}
				else
				{
					perror("fork");
					break ;
				}
			}
			curr = curr->next;
		}
		ft_free_cmd_list(cmd_list);
		cmd_list = NULL;
	}
	return (0);
}
