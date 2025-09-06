/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/06 11:57:05 by agarcia          ###   ########.fr       */
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
	if (fd_out != 1)
	{
		saved_stdout = dup(STDOUT_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
	if (fd_in != 0)
	{
		saved_stdin = dup(STDIN_FILENO);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (ft_strcmp(args[0], "echo") == 0)
		result = ft_echo(args);
	else if (ft_strcmp(args[0], "cd") == 0)
		result = ft_cd(args, *envp);
	else if (ft_strcmp(args[0], "pwd") == 0)
		result = ft_pwd(args);
	else if (ft_strcmp(args[0], "export") == 0)
		result = ft_export(args, envp);
	else if (ft_strcmp(args[0], "unset") == 0)
		result = ft_unset(args, envp);
	else if (ft_strcmp(args[0], "env") == 0)
		result = ft_env(*envp);
	else if (ft_strcmp(args[0], "exit") == 0)
		result = ft_exit(args);
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	return (1);
}
