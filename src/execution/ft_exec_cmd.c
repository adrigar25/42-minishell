/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 20:30:38 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;
	char	*error_msg;
	int		result;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	result = ft_handle_builtins(cmd, &envp);
		if (result == 0)
		return (0);
	if (cmd->infd != STDIN_FILENO)
	{
		dup2(cmd->infd, STDIN_FILENO);
		close(cmd->infd);
	}
	if (cmd->outfd != STDOUT_FILENO)
	{
		dup2(cmd->outfd, STDOUT_FILENO);
		close(cmd->outfd);
	}
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		if (result == -1)
			return (-1);
		path = get_cmd_path(cmd->argv[0]);
		if (!path)
		{
			printf(ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
			return (127);
		}
		if (execve(path, cmd->argv, envp) == -1)
		{
			perror("execve");
			free(path);
			return (1);
		}
		free(path);
		exit(127);
	}
}
