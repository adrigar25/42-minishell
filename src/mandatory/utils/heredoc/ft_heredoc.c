/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 18:05:39 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_read_heredoc_loop(int write_fd, const char *delimiter,
		t_data *data)
{
	char	*line;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, HEREDOC_PROMPT, ft_strlen(HEREDOC_PROMPT));
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_process_heredoc_line(write_fd, line, delimiter, data) != 0)
			break ;
	}
	return (0);
}

int	ft_heredoc(const char *delimiter, t_data *data)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	ft_read_heredoc_loop(pipefd[1], delimiter, data);
	close(pipefd[1]);
	return (pipefd[0]);
}
