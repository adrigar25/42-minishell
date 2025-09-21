/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_input_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/21 17:36:55 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_handle_syntax(char **argv, t_data *data)
{
	if (ft_check_syntax_errors(argv, data->argc))
	{
		data->last_exit_status = 2;
		ft_free_matrix(argv);
		if (!data->isatty)
			exit(2);
		return (1);
	}
	return (0);
}

t_cmd	*ft_process_input(char *input, t_data *data, int debug)
{
	t_cmd	*head;
	char	**argv;
	char	**expanded_argv;
	char	**wildcard_argv;
	t_cmd	*cmd_list;

	data->argc = ft_count_args(input);
	argv = ft_split_input(input, data->argc);
	free(input);
	if (ft_handle_syntax(argv, data))
		return (NULL);
	expanded_argv = ft_handle_env_expansion(argv, data);
	wildcard_argv = ft_handle_wildcards(expanded_argv, data);
	if (debug && wildcard_argv)
	{
		printf("Wildcard argv:\n");
		for (int i = 0; wildcard_argv[i]; i++)
			printf("  [%d]: %s\n", i, wildcard_argv[i]);
	}
	if (!wildcard_argv)
		wildcard_argv = expanded_argv;
	cmd_list = ft_parse_input(wildcard_argv, data);
	if (debug && cmd_list)
		ft_show_debug(argv, data->argc, wildcard_argv, cmd_list);
	ft_free_matrix(argv);
	if (expanded_argv != argv)
		ft_free_matrix(expanded_argv);
	if (wildcard_argv != expanded_argv && wildcard_argv != argv)
		ft_free_matrix(wildcard_argv);
	data->cmd_count = 0;
	head = cmd_list;
	while (head && ++data->cmd_count)
		head = head->next;
	return (cmd_list);
}
