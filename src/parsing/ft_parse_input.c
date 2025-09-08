/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 00:30:10 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/08 16:01:16 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_cmd	*ft_create_cmd_node(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->infd = STDIN_FD;
	new_cmd->outfd = STDOUT_FD;
	new_cmd->next = NULL;
	return (new_cmd);
}
static void	ft_add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int		i;
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
	i = 0;
	while (cmd->argv[i])
		i++;
	new_argv = malloc((i + 2) * sizeof(char *));
	if (!new_argv)
		return ;
	i = 0;
	while (cmd->argv[i])
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[i++] = arg;
	new_argv[i] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*ft_parse_input(char **argv, int argc)
{
	t_cmd *cmd_list;
	t_cmd *current_cmd;
	int i;

	if (!argv || argc == 0)
		return (NULL);

	cmd_list = ft_create_cmd_node();
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;

	i = 0;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "|") == 0)
		{
			current_cmd->next = ft_create_cmd_node();
			current_cmd = current_cmd->next;
		}
		else if (ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0)
		{

		}
		else
			ft_add_arg_to_cmd(current_cmd, argv[i]);

		i++;
	}

	return (cmd_list);
}