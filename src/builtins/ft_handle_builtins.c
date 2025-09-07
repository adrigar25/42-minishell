/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/07 00:57:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

static int	execute_builtin(char **args, char ***envp)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_echo(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args, *envp));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_pwd(args));
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args, envp));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unset(args, envp));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(*envp));
	if (ft_strcmp(args[0], "exit") == 0)
		return (ft_exit(args));
	return (1);
}

static void	handle_redirections(int *fd_in, int *fd_out, int *saved_stdin,
		int *saved_stdout)
{
	if (*fd_out != 1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		if (*saved_stdout == -1)
			perror("dup");
		if (dup2(*fd_out, STDOUT_FILENO) == -1)
			perror("dup2");
		close(*fd_out);
	}
	if (fd_in != 0)
	{
		*saved_stdin = dup(STDIN_FILENO);
		if (*saved_stdin == -1)
			perror("dup");
		if (dup2(*fd_in, STDIN_FILENO) == -1)
			perror("dup2");
		close(*fd_in);
	}
}

static void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdout != -1)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) == -1)
			perror("dup2");
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		if (dup2(saved_stdin, STDIN_FILENO) == -1)
			perror("dup2");
		close(saved_stdin);
	}
}

int	ft_handle_builtins(char **args, char ***envp)
{
	int	fd_in;
	int	fd_out;
	int	saved_stdout;
	int	saved_stdin;
	int	result;

	fd_in = 0;
	fd_out = 1;
	saved_stdout = -1;
	saved_stdin = -1;
	result = 0;
	if (!args || !args[0])
		return (0);
	if (!is_builtin(args[0]))
		return (0);
	ft_handle_redir(args, &fd_in, &fd_out);
	handle_redirections(&fd_in, &fd_out, &saved_stdin, &saved_stdout);
	result = execute_builtin(args, envp);
	restore_std_fds(saved_stdin, saved_stdout);
	return (1);
}
