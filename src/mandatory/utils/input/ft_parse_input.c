/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:57:27 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 13:50:43 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Parses the input arguments into a linked list of command
 *          structures, handling pipes and redirections.
 *
 * SPANISH: Parsea los argumentos de entrada en una lista enlazada de
 *          estructuras de comando, manejando tuberías y redirecciones.
 *
 * @param argv  The array of input argument strings. /
 *              El arreglo de cadenas de argumentos de entrada.
 *
 * @param data  Pointer to the shell data structure containing
 *              environment variables and other state information. /
 *              Puntero a la estructura de datos del shell que contiene
 *              variables de entorno y otra información de estado.
 *
 * @returns A pointer to the head of the linked list of command
 *          structures, or NULL on failure. /
 *          Un puntero a la cabeza de la lista enlazada de estructuras
 *          de comando, o NULL en caso de error.
 */
t_cmd	*ft_parse_input(char **argv, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		cmd_index;
	int		new_i;
	int		end;
	int		j;
	int		ret;
	t_cmd	*tmp;
	t_cmd	*tmp;

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
		// Pre-scan heredocs
		end = i;
		while (end < data->argc && argv[end] && ft_strcmp(argv[end], "|") != 0)
			end++;
		j = i;
		while (j < end)
		{
			if (argv[j] && ft_strcmp(argv[j], "<<") == 0 && argv[j + 1])
			{
				/* Process heredoc in pre-scan. If ft_redir returns
					-1 it means the
					heredoc was aborted (SIGINT) and we must abort parsing and return
					NULL so the caller drops the whole input line (single Ctrl+C). */
				ret = ft_redir(current_cmd, argv, j);
				if (ret == -1)
				{
					/* free partially built command list */
					while (cmd_list)
					{
						tmp = cmd_list->next;
						ft_free_matrix(cmd_list->argv);
						free(cmd_list);
						cmd_list = tmp;
					}
					return (NULL);
				}
				argv[j] = NULL;
				argv[j + 1] = NULL;
				j += 2;
				continue ;
			}
			j++;
		}
		while (i < data->argc && argv[i] == NULL)
			i++;
		if (i >= data->argc)
			break ;
		new_i = ft_process_token(&current_cmd, argv, i, &cmd_index);
		if (new_i == -1)
		{
			/* heredoc aborted via Ctrl+C: free partially built command list and
				return NULL so the caller (ft_process_input) will drop the line. */
			while (cmd_list)
			{
				tmp = cmd_list->next;
				ft_free_matrix(cmd_list->argv);
				free(cmd_list);
				cmd_list = tmp;
			}
			return (NULL);
		}
		if (new_i == i)
			i++;
		else
			i = new_i + 1;
	}
	return (cmd_list);
}
