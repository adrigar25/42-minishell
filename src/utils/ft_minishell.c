/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 17:21:53 by agarcia          ###   ########.fr       */
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
		cmd_list = ft_parse_input(argv, argc);
		free(argv);
		curr = cmd_list;
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			ft_exec_cmd(curr, envp);
			exit(1);
		}
		else if (pid < 0)
			perror("fork");
		waitpid(pid, &status, 0);
	}
	ft_free_cmd_list(cmd_list);
	return (0);
}
