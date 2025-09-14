/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:06:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:53:19 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
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
		return (ft_handle_error(1, EXIT_COMMAND_NOT_FOUND, cmd->argv[0], NULL));
	if (stat(path, &file_stat) == -1)
	{
		free(path);
		return (ft_handle_error(4, EXIT_COMMAND_NOT_FOUND, cmd->argv[0], NULL));
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		free(path);
		return (ft_handle_error(3, 126, cmd->argv[0], NULL));
	}
	if (access(path, X_OK) == -1)
	{
		free(path);
		return (ft_handle_error(2, 126, cmd->argv[0], NULL));
	}
	execve(path, cmd->argv, cmd->data->envp);
	perror("minishell: execve");
	free(path);
	return (EXIT_GENERAL_ERROR);
}
