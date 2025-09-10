/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 17:09:14 by agarcia          ###   ########.fr       */
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

int	ft_minishell(char **envp, int debug)
{
	char	*input;
	char	**argv;
	char	**expanded_argv;
	char	*prompt;
	t_cmd	*cmd_list;
	t_data	*data;
	pid_t	pid;
	t_cmd	*curr;
	int		i;
	int		status;
	int		builtin_result;
	int		is_interactive;
	pid_t	*pids;
	int		cmd_count;
	t_cmd	*tmp;
	int		cmd_index;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->last_exit_status = 0;
	ft_save_envp(&data->envp, envp);
	// Solo mostrar mensaje de bienvenida si es interactivo (tiene TTY)
	is_interactive = isatty(STDIN_FILENO);
	if (is_interactive)
	{
		ft_msg_start();
	}
	ft_init_signals();
	while (1)
	{
		cmd_list = NULL;
		prompt = ft_generate_prompt(data->envp);
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
		data->argc = ft_count_args(input);
		argv = ft_split_input(input, data->argc);
		if (debug)
		{
			printf("Argumentos recibidos:\n");
			for (i = 0; i < data->argc; i++)
				printf("argv[%d]: %s\n", i, argv[i]);
		}
		expanded_argv = ft_handle_env_expansion(argv, data);
		if (!expanded_argv)
			expanded_argv = argv;
		if (debug)
		{
			printf("Argumentos después de expandir:\n");
			for (i = 0; i < data->argc; i++)
				printf("argv[%d]: %s\n", i, expanded_argv[i]);
		}
		free(input);
		cmd_list = ft_parse_input(expanded_argv, data->argc);
		while (data->argc-- > 0)
			free(argv[data->argc]);
		free(argv);
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
		if (debug)
		{
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
		}
		// Verificar si hay un solo comando exit para manejo especial
		if (cmd_list && cmd_list->argv && strcmp(cmd_list->argv[0], "exit") == 0
			&& !cmd_list->next)
		{
			// Solo manejar exit si es un comando simple (no en pipe)
			builtin_result = ft_handle_builtins(cmd_list, &data);
			if (builtin_result == 0)
			{
				// Error en exit (ej: demasiados argumentos), continuar
				data->last_exit_status = 1;
			}
			// Para exit válido, ft_exit hace exit() internamente
			ft_free_cmd_list(cmd_list);
			cmd_list = NULL;
			continue ;
		}
		// Ejecutar todos los comandos en paralelo para pipes
		curr = cmd_list;
		pids = NULL;
		cmd_count = 0;
		// Contar comandos
		tmp = cmd_list;
		while (tmp)
		{
			cmd_count++;
			tmp = tmp->next;
		}
		// Allocar array para PIDs
		pids = malloc(sizeof(pid_t) * cmd_count);
		if (!pids)
		{
			perror("malloc");
			continue ;
		}
		cmd_index = 0;
		curr = cmd_list;
		// Fork todos los comandos
		while (curr)
		{
			builtin_result = ft_handle_builtins(curr, &data);
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
					if (ft_exec_cmd(curr, data) == -1)
					{
						ft_free_cmd_list(cmd_list);
						exit(EXIT_FAILURE);
					}
					exit(EXIT_SUCCESS);
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
			cmd_index++;
			curr = curr->next;
		}
		// Cerrar file descriptors en el proceso padre
		curr = cmd_list;
		while (curr)
		{
			if (curr->infd != STDIN_FILENO)
				close(curr->infd);
			if (curr->outfd != STDOUT_FILENO)
				close(curr->outfd);
			curr = curr->next;
		}
		i = 0;
		while (i < cmd_count)
		{
			if (pids[i] > 0)
			{
				waitpid(pids[i], &status, 0);
				if (WIFEXITED(status))
					data->last_exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					data->last_exit_status = 128 + WTERMSIG(status);
			}
			i++;
		}
		free(pids);
		ft_free_cmd_list(cmd_list);
		cmd_list = NULL;
	}
	return (0);
}
