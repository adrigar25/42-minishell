/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 00:20:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

static int	ft_init_data(t_data **data, char **envp)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (1);
	(*data)->last_exit_status = 0;
	(*data)->envp = ft_dupenv(envp);
	if (!(*data)->envp)
	{
		free(*data);
		return (1);
	}
	(*data)->isatty = isatty(STDIN_FILENO);
	return (0);
}

int	ft_minishell(char **envp, int debug)
{
	char	*input;
	t_cmd	*cmd_list;
	t_data	*data;
	int		exit_status;

	if (ft_init_data(&data, envp))
		return (1);
	if (data->isatty)
		ft_msg_start();
	ft_init_signals();
	exit_status = 0;
	while (ft_read_input(&input, data))
	{
		cmd_list = ft_process_input(input, data, debug);
		exit_status = ft_execute_pipeline(cmd_list, &data);
	}
	ft_free_matrix(data->envp);
	free(data);
	return (exit_status);
}
