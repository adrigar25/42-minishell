/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 16:46:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		ft_free_matrix(cmd_list->argv);
		free(cmd_list);
		cmd_list = tmp;
	}
}

static int	ft_is_redir(char *token)
{
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

static int	ft_process_segment(t_cmd **current_cmd, t_data *data, int *i,
		int *cmd_index)
{
	int	ret;

	if (data->argv[*i] != NULL)
	{
		if (*i >= data->argc)
			return (ERROR);
		if (ft_strcmp(data->argv[*i], "|") == 0)
		{
			if (ft_process_pipe(current_cmd, cmd_index,
					(*current_cmd)->data) == ERROR)
				return (ERROR);
		}
		else if (ft_is_redir(data->argv[*i]))
		{
			ret = ft_redir(*current_cmd, data->argv, *i);
			if (ret == -1)
				return (ERROR);
			*i = ret;
		}
		else if (data->argv[*i] && data->argv[*i][0] != '\0')
			ft_add_arg_to_cmd(*current_cmd, ft_remove_quotes(data->argv[*i]));
	}
	(*i)++;
	return (SUCCESS);
}

t_cmd	*ft_parse_input(t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		cmd_index;
	int		ret;

	i = 0;
	cmd_index = 0;
	ret = 0;
	if (!data->argv || !data || data->argc == 0)
		return (NULL);
	cmd_list = ft_create_cmd_node(0);
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	cmd_list->data = data;
	while (i < data->argc && ret == SUCCESS)
		ret = ft_process_segment(&current_cmd, data, &i, &cmd_index);
	if (ret == ERROR)
	{
		free_cmd_list(cmd_list);
		return (NULL);
	}
	return (cmd_list);
}
