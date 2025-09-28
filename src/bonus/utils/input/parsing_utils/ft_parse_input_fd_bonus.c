/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_fd_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:31 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 22:27:16 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

int	ft_get_fd_from_token(char *token, char *filename)
{
	if (ft_strcmp(token, "<") == 0)
		return (ft_handle_infile(filename));
	else if (ft_strcmp(token, ">") == 0)
		return (ft_handle_outfile(filename, 0));
	else if (ft_strcmp(token, ">>") == 0)
		return (ft_handle_outfile(filename, 1));
	else
		return (ft_handle_heredoc(filename));
}

void	ft_assign_fd(t_cmd *cmd, char *token, int fd)
{
	if (ft_strcmp(token, "<") == 0 || ft_strcmp(token, "<<") == 0)
	{
		if (cmd->infd != STDIN_FILENO)
			close(cmd->infd);
		cmd->infd = fd;
	}
	else
	{
		if (cmd->outfd != STDOUT_FILENO)
			close(cmd->outfd);
		cmd->outfd = fd;
	}
}
