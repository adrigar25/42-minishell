/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 23:02:44 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

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

static int	ft_is_operator(char *token)
{
	return (!ft_strcmp(token, "|") || !ft_strcmp(token, "||")
		|| !ft_strcmp(token, "&&"));
}

static int	ft_process_segment(t_cmd **current_cmd, t_data *data, int *i,
		int *cmd_index)
{
	int	ret;

	if (data->argv[*i] != NULL)
	{
		if (*i >= data->argc)
			return (ERROR);
		if (ft_is_operator(data->argv[*i]))
		{
			if (ft_process_op(current_cmd, data->argv[*i], cmd_index,
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
		else
			ft_add_arg_to_cmd(*current_cmd, ft_remove_quotes(data->argv[*i]));
	}
	(*i)++;
	return (SUCCESS);
}

/**
 * Parses input arguments into a command list structure.
 * Handles pipes (|), logical operators (&&, ||), and redirections.
 * Each command node contains arguments, file descriptors, and operator type.
 *
 * Analiza los argumentos de entrada en una estructura de lista de comandos.
 * Maneja pipes (|), operadores lógicos (&&, ||) y redirecciones.
 * Cada nodo de comando contiene argumentos, descriptores de archivo y tipo
 * de operador.
 *
 * @param data  Shell data structure with argv array and argc count.
 * @returns     Pointer to command list head, or NULL on error.
 */
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
