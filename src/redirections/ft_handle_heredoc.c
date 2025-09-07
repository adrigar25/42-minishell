/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/06 16:54:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_handle_here_doc(char *limiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;

	if (!limiter)
		return (-1);
	len = ft_strlen(limiter);
	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		write(STDOUT_FD, HEREDOC_PROMPT, ft_strlen(HEREDOC_PROMPT));
		line = ft_get_next_line(STDIN_FD);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, len) == 0 && line[len] == '\n')
		{
			free(line);
			break ;
		}
		write(pipefd[1], line, ft_strlen(line));
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
