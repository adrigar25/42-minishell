/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 20:44:19 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;
	char	*error_msg;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	if (ft_handle_builtins(cmd, &envp) == 0)
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
		path = get_cmd_path(cmd->argv[0]);
	if (!path || access(path, X_OK) == -1)
	{
		if (path)
		{
			error_msg = ERROR_PERMISSION_DENIED;
			ft_putstr_error(error_msg);
			free(error_msg);
		}
		else
		{
			error_msg = ft_strjoin(ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
			ft_putstr_error(error_msg);
			free(error_msg);
		}
		free(path);
		exit(127);
	}
	// Ejecutar comando (esto reemplaza el proceso)
	execve(path, cmd->argv, envp);
	perror("minishell: execve");
	free(path);
	exit(127);
}
