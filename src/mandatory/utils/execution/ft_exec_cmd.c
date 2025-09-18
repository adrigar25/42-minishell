/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:06:20 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 16:17:41 by agarcia          ###   ########.fr       */
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
		return (ft_handle_error(3, EXIT_PERMISSION_DENIED, cmd->argv[0], NULL));
	}
	if (access(path, X_OK) == -1)
	{
		free(path);
		return (ft_handle_error(2, EXIT_PERMISSION_DENIED, cmd->argv[0], NULL));
	}
	execve(path, cmd->argv, cmd->data->envp);
	perror("minishell: execve");
	free(path);
	return (EXIT_FAILURE);
}
