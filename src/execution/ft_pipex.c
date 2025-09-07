/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:37:42 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/07 01:16:24 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_handle_pipe_error(char *msg, char ***cmds)
{
	perror(msg);
	if (cmds)
		free_split_strings(cmds);
	return (1);
}

int	ft_pipex(const char **argv, int fd_in, char **envp)
{
	int		pipefds[2];
	char	***cmds;
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		in_fd;
	int		out_fd;

	prev_fd = fd_in;
	i = 0;
	cmds = ft_split_strings(argv, "|");
	if (!cmds)
		return (1);
	while (cmds && cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefds) == -1)
			return (ft_handle_pipe_error("pipe", cmds));
		pid = fork();
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			if (i == 0)
				in_fd = (fd_in != -1) ? fd_in : 0;
			else
				in_fd = prev_fd;
			if (cmds[i + 1])
				out_fd = pipefds[1];
			else
				out_fd = 1;
			ft_exec_cmd(cmds[i], in_fd, out_fd, envp);
			exit(1);
		}
		if (prev_fd != -1 && prev_fd != fd_in)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(pipefds[1]);
			prev_fd = pipefds[0];
		}
		i++;
	}
	if (fd_in != -1)
		close(fd_in);
	while (i-- > 0)
		wait(NULL);
	free_split_strings(cmds);
	return (0);
}
