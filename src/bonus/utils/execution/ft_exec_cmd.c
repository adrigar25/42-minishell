/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:06:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 14:42:26 by agarcia          ###   ########.fr       */
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

	if (!cmd)
		return (1);
	/* If there's no argv (redirection-only command),
		treat as successful no-op */
	if (!cmd->argv)
		return (0);
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
	if (!cmd->argv[0])
		return (0);
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = get_cmd_path(cmd->argv[0]);
	if (!path)
	{
		ft_fprintf(2, ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	if (stat(path, &file_stat) == -1)
	{
		ft_fprintf(2, ERROR_NO_SUCH_FILE, cmd->argv[0]);
		free(path);
		return (EXIT_COMMAND_NOT_FOUND);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		ft_fprintf(2, ERROR_IS_A_DIRECTORY, cmd->argv[0]);
		free(path);
		return (EXIT_PERMISSION_DENIED);
	}
	if (access(path, X_OK) == -1)
	{
		ft_fprintf(2, ERROR_PERMISSION_DENIED, cmd->argv[0]);
		free(path);
		return (EXIT_PERMISSION_DENIED);
	}
	execve(path, cmd->argv, cmd->data->envp);
	perror("minishell: execve");
	free(path);
	return (EXIT_GENERAL_ERROR);
}
