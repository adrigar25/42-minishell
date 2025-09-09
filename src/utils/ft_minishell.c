/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 19:22:03 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	char	**argv;
	int		argc;
	char	*input;
	char	*prompt;
	t_cmd	*cmd_list;
	t_cmd	*curr;
	int		i;
	pid_t	pid;
	int		status;
	int		pipefd[2];
	char	**envp_cpy;

	ft_save_envp(&envp_cpy, envp);
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
		argv = ft_split_input(input, argc);
		free(input);
		for (i = 0; i < argc; i++)
			printf("argv[%d]: %s\n", i, argv[i]);
		cmd_list = ft_parse_input(argv, argc);
		for (i = 0; i < argc; i++)
			free(argv[i]);
		free(argv);
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
		curr = cmd_list;
		while (curr)
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				ft_exec_cmd(curr, envp);
				exit(1);
			}
			else if (pid > 0)
			{
				// Cerrar file descriptors en el padre
				if (curr->infd != STDIN_FILENO)
					close(curr->infd);
				if (curr->outfd != STDOUT_FILENO)
					close(curr->outfd);
				// Esperar a que termine este proceso
				waitpid(pid, &status, 0);
			}
			else
			{
				perror("fork");
				break ;
			}
			curr = curr->next;
		}
		ft_free_cmd_list(cmd_list);
	}
	return (0);
}
