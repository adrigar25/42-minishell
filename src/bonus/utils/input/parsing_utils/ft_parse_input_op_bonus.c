/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_op_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:52:24 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 14:01:34 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

int	ft_handle_pipe(t_cmd **current_cmd, int *pipefd)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	if ((*current_cmd)->outfd == STDOUT_FILENO)
		(*current_cmd)->outfd = pipefd[1];
	else
		close(pipefd[1]);
	return (0);
}

int	ft_process_op(t_cmd **current_cmd, char *arg, int *cmd_index, t_data *data)
{
	int		pipefd[2];
	t_cmd	*new_cmd;

	new_cmd = ft_create_cmd_node(*cmd_index + 1);
	if (!new_cmd)
		return (-1);
	(*cmd_index)++;
	(*current_cmd)->next = new_cmd;
	new_cmd->data = data;
	new_cmd->op = OP_NONE;
	if (ft_strcmp(arg, "|") == 0)
		new_cmd->op = OP_PIPE;
	else if (ft_strcmp(arg, "||") == 0)
		new_cmd->op = OP_OR;
	else if (ft_strcmp(arg, "&&") == 0)
		new_cmd->op = OP_AND;
	if (new_cmd->op == OP_PIPE)
	{
		if (ft_handle_pipe(current_cmd, pipefd) == -1)
			return (-1);
		(*current_cmd)->outfd = pipefd[1];
		new_cmd->infd = pipefd[0];
	}
	*current_cmd = new_cmd;
	return (0);
}
