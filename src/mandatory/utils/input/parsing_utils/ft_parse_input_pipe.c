/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:52:24 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:52:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_process_pipe(t_cmd **current_cmd, int *cmd_index, t_data *data)
{
	int		pipefd[2];
	t_cmd	*new_cmd;

	if (pipe(pipefd) == -1)
		return (-1);
	new_cmd = ft_create_cmd_node(*cmd_index + 1);
	if (!new_cmd)
		return (-1);
	(*cmd_index)++;
	(*current_cmd)->next = new_cmd;
	new_cmd->data = data;
	if ((*current_cmd)->outfd == STDOUT_FILENO)
		(*current_cmd)->outfd = pipefd[1];
	else
		close(pipefd[1]);
	new_cmd->infd = pipefd[0];
	*current_cmd = new_cmd;
	return (0);
}
