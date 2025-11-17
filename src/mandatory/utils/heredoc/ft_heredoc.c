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

int	ft_process_heredoc_line(int write_fd, char *line, const char *delimiter,
		t_data *data)
{
	char	*expanded;
	int		has_nl;

	has_nl = (line[ft_strlen(line) - 1] == '\n');
	if (ft_strncmp(line, delimiter, ft_strlen(line) - has_nl) == 0
		&& ft_strlen(line) - has_nl == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	expanded = ft_process_arg(line, data);
	if (!expanded)
		return (-1);
	write(write_fd, expanded, ft_strlen(expanded));
	free(expanded);
	free(line);
	return (0);
}

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
