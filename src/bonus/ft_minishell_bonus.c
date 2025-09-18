/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 14:14:43 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int	ft_minishell(char **envp, int debug)
{
	char	*input;
	t_cmd	*cmd_list;
	t_data	*data;
	int		exit_status;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	data->last_exit_status = 0;
	ft_dupenv(&data->envp, envp);
	data->isatty = isatty(STDIN_FILENO);
	if (data->isatty)
		ft_msg_start();
	ft_init_signals();
	exit_status = 0;
	while (ft_read_input(&input, data))
	{
		cmd_list = ft_process_input(input, data, debug);
		if (cmd_list)
			exit_status = ft_execute_pipeline(cmd_list, &data);
	}
	ft_free_matrix(data->envp);
	free(data);
	return (exit_status);
}
