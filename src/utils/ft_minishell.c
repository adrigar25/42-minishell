/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 19:01:54 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

static void	ft_free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		ft_free_char_array(cmd_list->argv);
		free(cmd_list);
		cmd_list = tmp;
	}
}
static int	ft_read_input(char **input, t_data *data)
{
	char	*prompt;

	while (1)
	{
		if (data->isatty)
		{
			prompt = ft_generate_prompt(data->envp);
			*input = readline(prompt);
			free(prompt);
		}
		else
			*input = readline("minishell> ");
		if (!*input)
			return (0);
		if (**input)
		{
			if (data->isatty)
				add_history(*input);
			return (1);
		}
		free(*input);
	}
}

static void	ft_finish_execution(pid_t *pids, int cmd_count, t_cmd *cmd_list,
		t_data *data)
{
	int	i;
	int	status;

	i = cmd_count - 1;
	while (i >= 0)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFEXITED(status))
				data->last_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				data->last_exit_status = 128 + WTERMSIG(status);
		}
		i--;
	}
	free(pids);
	ft_free_cmd_list(cmd_list);
	cmd_list = NULL;
}

static int	ft_process_input(char *input, t_data *data, t_cmd **cmd_list,
		int debug)
{
	t_cmd	*head;
	char	**argv;
	char	**expanded_argv;

	data->argc = ft_count_args(input);
	argv = ft_split_input(input, data->argc);
	free(input);
	if (ft_check_syntax_errors(argv, data->argc))
	{
		data->last_exit_status = 2;
		ft_free_char_array(argv);
		if (!data->isatty)
			exit(2);
		return (0);
	}
	expanded_argv = ft_handle_env_expansion(argv, data);
	*cmd_list = ft_parse_input(expanded_argv, data);
	if (debug && *cmd_list)
		ft_show_debug(argv, data->argc, expanded_argv, *cmd_list);
	ft_free_char_array(argv);
	if (expanded_argv != argv)
		ft_free_char_array(expanded_argv);
	data->cmd_count = 0;
	head = *cmd_list;
	while (head)
	{
		data->cmd_count++;
		head = head->next;
	}
	return (1);
}

int	ft_minishell(char **envp, int debug)
{
	char	*input;
	char	**argv;
	char	**expanded_argv;
	char	*prompt;
	t_cmd	*cmd_list;
	t_data	*data;
	pid_t	pid;
	int		i;
	int		status;
	int		builtin_result;
	pid_t	*pids;
	t_cmd	*head;
	int		cmd_index;
	int		exit_status;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->last_exit_status = 0;
	ft_save_envp(&data->envp, envp);
	data->isatty = isatty(STDIN_FILENO);
	if (data->isatty)
		ft_msg_start();
	ft_init_signals();
	while (ft_read_input(&input, data))
	{
		if (!ft_process_input(input, data, &cmd_list, debug))
			continue ;
		head = cmd_list;
		pids = malloc(sizeof(pid_t) * data->cmd_count);
		if (!pids)
			continue ;
		cmd_index = 0;
		cmd_list = head;
		while (cmd_list)
		{
			if (cmd_list->has_error)
			{
				pids[cmd_index] = -1;
				cmd_index++;
				cmd_list = cmd_list->next;
				continue ;
			}
			if (cmd_list->next)
			{
				pid = fork();
				if (pid == 0)
				{
					signal(SIGINT, SIG_DFL);
					signal(SIGQUIT, SIG_DFL);
					signal(SIGPIPE, SIG_DFL);
					if (cmd_list->infd != STDIN_FILENO)
					{
						dup2(cmd_list->infd, STDIN_FILENO);
						close(cmd_list->infd);
					}
					if (cmd_list->outfd != STDOUT_FILENO)
					{
						dup2(cmd_list->outfd, STDOUT_FILENO);
						close(cmd_list->outfd);
					}
					builtin_result = ft_handle_builtins(cmd_list, &data);
					if (builtin_result != -1)
						exit(builtin_result);
					else
						exit(ft_exec_cmd(cmd_list, data));
				}
				else if (pid > 0)
				{
					pids[cmd_index] = pid;
					if (cmd_list->infd != STDIN_FILENO)
						close(cmd_list->infd);
					if (cmd_list->outfd != STDOUT_FILENO)
						close(cmd_list->outfd);
				}
				else
				{
					perror("fork");
					break ;
				}
			}
			else
			{
				if (cmd_list->infd != STDIN_FILENO
					|| cmd_list->outfd != STDOUT_FILENO)
				{
					pid = fork();
					if (pid == 0)
					{
						signal(SIGINT, SIG_DFL);
						signal(SIGQUIT, SIG_DFL);
						signal(SIGPIPE, SIG_DFL);
						if (cmd_list->infd != STDIN_FILENO)
						{
							dup2(cmd_list->infd, STDIN_FILENO);
							close(cmd_list->infd);
						}
						if (cmd_list->outfd != STDOUT_FILENO)
						{
							dup2(cmd_list->outfd, STDOUT_FILENO);
							close(cmd_list->outfd);
						}
						builtin_result = ft_handle_builtins(cmd_list, &data);
						if (builtin_result != -1)
						{
							exit(builtin_result);
						}
						else
						{
							exit_status = ft_exec_cmd(cmd_list, data);
							exit(exit_status);
						}
					}
					else if (pid > 0)
					{
						pids[cmd_index] = pid;
						if (cmd_list->infd != STDIN_FILENO)
							close(cmd_list->infd);
						if (cmd_list->outfd != STDOUT_FILENO)
							close(cmd_list->outfd);
					}
					else
					{
						perror("fork");
						break ;
					}
				}
				else
				{
					builtin_result = ft_handle_builtins(cmd_list, &data);
					if (builtin_result != -1)
					{
						data->last_exit_status = builtin_result;
						pids[cmd_index] = -1;
					}
					else
					{
						pid = fork();
						if (pid == 0)
						{
							signal(SIGINT, SIG_DFL);
							signal(SIGQUIT, SIG_DFL);
							signal(SIGPIPE, SIG_DFL);
							exit_status = ft_exec_cmd(cmd_list);
							if (exit_status == -1)
							{
								ft_free_cmd_list(cmd_list);
								exit(EXIT_FAILURE);
							}
							exit(exit_status);
						}
						else if (pid > 0)
						{
							pids[cmd_index] = pid;
						}
						else
						{
							perror("fork");
							break ;
						}
					}
				}
			}
			cmd_index++;
			cmd_list = cmd_list->next;
		}
		cmd_list = head;
		ft_finish_execution(pids, data->cmd_count, cmd_list, data);
	}
	return (0);
}
