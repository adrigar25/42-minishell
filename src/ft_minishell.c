/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 01:28:50 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

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
	int		exit_status;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->last_exit_status = 0;
	ft_cpyenv(&data->envp, envp);
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
		cmd_list = head;
		while (cmd_list)
		{
			if (cmd_list->has_error)
			{
				if (cmd_list->next)
					ft_execute_error_command(cmd_list, head, pids);
				else
					ft_skip_error_cmd(cmd_list, data, pids);
				cmd_list = cmd_list->next;
				continue ;
			}
			if (!cmd_list->next && cmd_list->infd == STDIN_FILENO
				&& cmd_list->outfd == STDOUT_FILENO)
			{
				builtin_result = ft_handle_builtins(cmd_list, &data);
				if (builtin_result != -1)
				{
					data->last_exit_status = builtin_result;
					pids[cmd_list->index] = -1;
					cmd_list = cmd_list->next;
					continue ;
				}
			}
			if (ft_execute_command(cmd_list, head, pids, &data) == -1)
				break ;
			cmd_list = cmd_list->next;
		}
		cmd_list = head;
		ft_finish_execution(pids, data->cmd_count, cmd_list, data);
	}
	exit_status = data->last_exit_status;
	ft_free_char_array(data->envp);
	free(data);
	return (exit_status);
}
