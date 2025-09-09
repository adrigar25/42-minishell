/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 20:31:45 by adriescr         ###   ########.fr       */
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

		cmd_list = ft_parse_input(argv, argc);

		while (argc-- > 0)
			free(argv[argc]);
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

		if (cmd_list && cmd_list->argv && strcmp(cmd_list->argv[0], "exit") == 0)
		{
			ft_free_cmd_list(cmd_list);
			break;
		}
		while (curr)
		{
			pid = fork();
			if (pid == 0)
			{
				signal(SIGINT, SIG_DFL);
				signal(SIGQUIT, SIG_DFL);
				if (ft_exec_cmd(cmd_list, envp) == -1)
			{
				ft_free_cmd_list(cmd_list);
				exit(EXIT_FAILURE);
			}
				exit(EXIT_SUCCESS);
			}
			else if (pid > 0)
			{
				// Cerrar file descriptors en el padre
				if (curr->infd != STDIN_FILENO)
					close(curr->infd);
				if (curr->outfd != STDOUT_FILENO)
					close(curr->outfd);
				// Esperar a que termine este proceso
		else
					waitpid(pid, NULL, 0);
			}
			else
			{
				perror("fork");
				break ;
			}
			curr = curr->next;

			ft_free_cmd_list(cmd_list);
		cmd_list = NULL;
	}
	}
	return (0);
}
