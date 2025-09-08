/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 01:09:37 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_handle_builtins(cmd->argv, &envp) == 0)
		return (0);
	if (cmd->infd != STDIN_FD)
	{
		dup2(cmd->infd, STDIN_FD);
		close(cmd->infd);
	}
	if (cmd->outfd != STDOUT_FD)
	{
		dup2(cmd->outfd, STDOUT_FD);
		close(cmd->outfd);
	}
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = get_cmd_path(cmd->argv[0]);
	if (!path || access(path, X_OK) == -1)
	{
		if (path)
			printf(ERROR_PERMISSION_DENIED, cmd->argv[0]);
		else
			printf(ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
		free(path);
		exit(127);
	}
	execve(path, cmd->argv, envp);
	perror("minishell: execve");
	free(path);
	exit(127);
}
