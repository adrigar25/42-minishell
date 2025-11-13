/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/13 18:09:14 by adriescr         ###   ########.fr       */
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

static int	handle_heredocs(t_cmd *current_cmd, char **argv, int start, int end)
{
	int	j;
	int	ret;

	j = start;
	while (j < end)
	{
		if (argv[j] && ft_strcmp(argv[j], "<<") == 0 && argv[j + 1])
		{
			ret = ft_redir(current_cmd, argv, j);
			if (ret == -1)
				return (-1);
			argv[j] = NULL;
			argv[j + 1] = NULL;
			j += 2;
			continue ;
		}
		j++;
	}
	return (0);
}

static int	process_segment(t_parse_ctx *ctx)
{
	int	end;
	int	new_i;

	end = ft_find_segment_end(ctx->argv, ctx->data->argc, *ctx->i);
	if (handle_heredocs(*ctx->current_cmd, ctx->argv, *ctx->i, end) == -1)
		return (-1);
	*ctx->i = ft_skip_nulls(ctx->argv, ctx->data->argc, *ctx->i);
	if (*ctx->i >= ctx->data->argc)
		return (1);
	new_i = ft_process_token(ctx->current_cmd, ctx->argv, *ctx->i,
			ctx->cmd_index);
	if (new_i == -1)
		return (-1);
	if (new_i == *ctx->i)
		(*ctx->i)++;
	else
		*ctx->i = new_i + 1;
	return (0);
}

static t_cmd	*init_parse_state(t_data *data, t_cmd **current_cmd, int *i,
		int *cmd_index)
{
	t_cmd	*cmd_list;

	*cmd_index = 0;
	cmd_list = ft_create_cmd_node(*cmd_index);
	if (!cmd_list)
		return (NULL);
	*current_cmd = cmd_list;
	cmd_list->data = data;
	*i = 0;
	return (cmd_list);
}

t_cmd	*ft_parse_input(char **argv, t_data *data)
{
	t_cmd		*cmd_list;
	t_cmd		*current_cmd;
	int			i[3];
	t_parse_ctx	ctx;

	if (!argv || !data || data->argc == 0)
		return (NULL);
	cmd_list = init_parse_state(data, &current_cmd, &i[0], &i[1]);
	if (!cmd_list)
		return (NULL);
	while (i[0] < data->argc)
	{
		ctx = (t_parse_ctx){&current_cmd, argv, data, &i[0], &i[1]};
		i[2] = process_segment(&ctx);
		if (i[2] == 1)
			break ;
	}
	if (i[2] == -1)
	{
		free_cmd_list(cmd_list);
		return (NULL);
	}
	return (cmd_list);
}
