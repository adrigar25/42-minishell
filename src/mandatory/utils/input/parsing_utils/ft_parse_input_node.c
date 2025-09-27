/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_node.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:29 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:51:29 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

t_cmd	*ft_create_cmd_node(int index)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->infd = STDIN_FILENO;
	new_cmd->outfd = STDOUT_FILENO;
	new_cmd->has_error = 0;
	new_cmd->index = index;
	new_cmd->next = NULL;
	return (new_cmd);
}

void	ft_add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_argv;

	if (!cmd->argv)
	{
		cmd->argv = malloc(2 * sizeof(char *));
		if (!cmd->argv)
			return ;
		cmd->argv[0] = arg;
		cmd->argv[1] = NULL;
		return ;
	}
	count = ft_count_arg(cmd->argv);
	new_argv = malloc((count + 2) * sizeof(char *));
	if (!new_argv)
		return ;
	ft_copy_args(cmd->argv, new_argv);
	new_argv[count] = arg;
	new_argv[count + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}
