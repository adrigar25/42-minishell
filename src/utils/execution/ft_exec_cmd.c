/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:06:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 00:48:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <sys/stat.h>

int	ft_exec_cmd(t_cmd *cmd)
{
	char		*path;
	char		*error_msg;
	struct stat	file_stat;
	int			i;
	int			j;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	// Remove empty arguments and shift array
	i = 0;
	j = 0;
	while (cmd->argv[i])
	{
		if (ft_strlen(cmd->argv[i]) > 0)
		{
			if (i != j)
				cmd->argv[j] = cmd->argv[i];
			j++;
		}
		i++;
	}
	cmd->argv[j] = NULL;
	// If no valid command after removing empty strings
	if (!cmd->argv[0])
		return (0);
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = get_cmd_path(cmd->argv[0]);
	if (!path)
	{
		error_msg = ft_strjoin("\033minishell: ", cmd->argv[0]);
		error_msg = ft_strjoin(error_msg, ": command not found\033\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	if (stat(path, &file_stat) == -1)
	{
		error_msg = ft_strjoin("\033minishell: ", cmd->argv[0]);
		error_msg = ft_strjoin(error_msg, ": No such file or directory\033\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		free(path);
		exit(EXIT_COMMAND_NOT_FOUND);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		error_msg = ft_strjoin("\033minishell: ", cmd->argv[0]);
		error_msg = ft_strjoin(error_msg, ": is a directory\033\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		free(path);
		exit(EXIT_PERMISSION_DENIED);
	}
	if (access(path, X_OK) == -1)
	{
		ft_putstr_error(ERROR_PERMISSION_DENIED);
		free(path);
		exit(EXIT_PERMISSION_DENIED);
	}
	execve(path, cmd->argv, cmd->data->envp);
	perror("minishell: execve");
	free(path);
	exit(EXIT_GENERAL_ERROR);
}
