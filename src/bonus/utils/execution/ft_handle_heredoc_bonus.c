/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	ft_handle_heredoc(const char *delimiter)
{
	int		pipefd[2];
	char	*line;
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	if (pipe(pipefd) == -1)
		return (-1);
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, "heredoc> ", 9);
		line = NULL;
		nread = getline(&line, &len, stdin);
		if (nread == -1 || !line)
			break ;
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line[nread - 1] = '\n';
		write(pipefd[1], line, nread);
		free(line);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}
