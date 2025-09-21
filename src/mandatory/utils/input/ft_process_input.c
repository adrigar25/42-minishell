/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_input.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 01:04:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_count_cmds(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*head;

	count = 0;
	head = cmd_list;
	while (head && ++count)
		head = head->next;
	return (count);
}

t_cmd	*ft_process_input(char *input, t_data *data, int debug)
{
	t_cmd	*head;
	char	**argv;
	t_cmd	*cmd_list;

	data->argc = ft_count_args(input);
	argv = ft_split_input(input, data->argc);
	free(input);
	if (ft_check_syntax_errors(argv, data->argc))
	{
		ft_free_matrix(argv);
		data->last_exit_status = 2;
		return (NULL);
	}
	argv = ft_handle_env_expansion(argv, data);
	cmd_list = ft_parse_input(argv, data);
	if (debug && cmd_list && argv)
		ft_show_debug(argv, data->argc, argv, cmd_list);
	ft_free_matrix(argv);
	data->cmd_count = ft_count_cmds(cmd_list);
	return (cmd_list);
}
