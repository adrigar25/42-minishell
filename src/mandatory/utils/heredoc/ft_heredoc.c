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
		t_data *data, int expand)
{
	char		*line;
	t_hdoc_ctx	ctx;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, HEREDOC_PROMPT, ft_strlen(HEREDOC_PROMPT));
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		ctx.data = data;
		ctx.expand = expand;
		if (ft_process_heredoc_line(write_fd, line, delimiter, &ctx) != 0)
			break ;
	}
	return (0);
}

static void	ft_heredoc_child(int write_fd, const char *delimiter, t_data *data,
		int expand)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	ft_read_heredoc_loop(write_fd, delimiter, data, expand);
	close(write_fd);
	_exit(0);
}

static int	ft_heredoc_parent(pid_t pid, int read_fd, int write_fd,
		struct termios *orig_term)
{
	int	status;

	close(write_fd);
	if (waitpid(pid, &status, 0) == -1)
	{
		close(read_fd);
		signal(SIGINT, sigint_handler);
		return (-1);
	}
	signal(SIGINT, sigint_handler);
	if (orig_term)
		tcsetattr(STDIN_FILENO, TCSANOW, orig_term);
	rl_on_new_line();
	rl_replace_line("", 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(read_fd);
		write(1, "\n", 1);
		return (-2);
	}
	return (read_fd);
}

int	ft_heredoc(const char *delimiter, t_data *data, int expand)
{
	int				pipefd[2];
	pid_t			pid;
	struct termios	orig_term;

	if (pipe(pipefd) == -1)
		return (-1);
	if (data && data->isatty)
	{
		tcgetattr(STDIN_FILENO, &orig_term);
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid < 0)
			return (close(pipefd[0]), close(pipefd[1]), -1);
		if (pid == 0)
		{
			close(pipefd[0]);
			ft_heredoc_child(pipefd[1], delimiter, data, expand);
		}
		return (ft_heredoc_parent(pid, pipefd[0], pipefd[1], &orig_term));
	}
	ft_read_heredoc_loop(pipefd[1], delimiter, data, expand);
	close(pipefd[1]);
	return (pipefd[0]);
}
