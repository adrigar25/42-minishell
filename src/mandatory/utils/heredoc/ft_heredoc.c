/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 17:42:14 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

static int	hdoc_process(int fd, char *line, const char *del, t_data *data)
{
	char	*exp;
	int		nl;

	nl = (line[ft_strlen(line) - 1] == '\n');
	if (!ft_strncmp(line, del, ft_strlen(line) - nl) && ft_strlen(line)
		- nl == ft_strlen(del))
	{
		free(line);
		return (1);
	}
	exp = ft_process_arg(line, data);
	if (!exp)
		return (free(line), -1);
	write(fd, exp, ft_strlen(exp));
	write(fd, "\n", 1);
	free(exp);
	return (free(line), 0);
}

static int	hdoc_loop(int fd, const char *del, t_data *data)
{
	char	*line;

	while (1)
	{
		line = readline(HEREDOC_PROMPT);
		if (!line)
			return (-1);
		if (hdoc_process(fd, line, del, data) != 0)
			break ;
	}
	return (0);
}

static void	hdoc_child(int fd, const char *del, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	hdoc_loop(fd, del, data);
	close(fd);
	_exit(0);
}

static int	hdoc_parent(pid_t pid, int rd, int wr, t_data *data)
{
	int	status;

	close(wr);
	waitpid(pid, &status, 0);
	ft_init_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(rd);
		write(1, "\n", 1);
		data->last_exit_status = 130;
		return (-1);
	}
	return (rd);
}

int	ft_heredoc(const char *del, t_data *data)
{
	int		pipefd[2];
	pid_t	pid;

	if (pipe(pipefd) == -1)
		return (-1);
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
		return (close(pipefd[0]), close(pipefd[1]), -1);
	if (pid == 0)
	{
		close(pipefd[0]);
		hdoc_child(pipefd[1], del, data);
	}
	return (hdoc_parent(pid, pipefd[0], pipefd[1], data));
}
