/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:06:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/26 21:59:07 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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

	if (!cmd)
		return ;
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

t_cmd	*ft_parse_input(char **argv, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		cmd_index;
	int		new_i;

	if (!argv || data->argc == 0)
		return (NULL);
	cmd_index = 0;
	cmd_list = ft_create_cmd_node(cmd_index);
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	current_cmd->data = data;
	i = 0;
	while (i < data->argc)
	{
		new_i = ft_process_token(&current_cmd, argv, i, &cmd_index, data);
		if (new_i == i)
			i++;
		else
			i = new_i + 1;
	}
	return (cmd_list);
}
