/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redir_error_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 19:23:16 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/20 19:35:39 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

int	ft_is_ambiguous_redirect(char *filename)
{
	int	i;

	if (!filename || !filename[0])
		return (1);
	i = 0;
	while (filename[i])
	{
		if (filename[i] == ' ' || filename[i] == '\t')
			return (1);
		i++;
	}
	return (0);
}

int	ft_check_ambiguous(t_cmd *cmd, char **argv, int i, char *expanded_arg)
{
	char	*clean_arg;
	char	*original_arg;

	original_arg = argv[i + 1];
	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	if (ft_strcmp(argv[i], "<<") != 0 && ((!original_arg || !expanded_arg)
			|| ft_strcmp(original_arg, expanded_arg) != 0)
		&& ft_is_ambiguous_redirect(expanded_arg))
	{
		if (clean_arg != argv[i + 1])
			free(clean_arg);
		ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, original_arg);
		cmd->data->last_exit_status = 1;
		cmd->has_error = 1;
		free(expanded_arg);
		return (1);
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (0);
}

int	ft_handle_fd_error(t_cmd *cmd, int fd_ret)
{
	if (fd_ret == -2)
		return (-1);
	if (fd_ret == -1)
	{
		cmd->data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	return (0);
}

int	ft_handle_wildcard_sentinel(t_cmd *cmd, char *expanded_arg)
{
	if (!expanded_arg)
		return (0);
	if (ft_strncmp(expanded_arg, "!NOFILE!", 8) == 0 || ft_strcmp(expanded_arg,
			"!AMB_REDIRECT!") == 0)
	{
		if (ft_strncmp(expanded_arg, "!NOFILE!", 8) == 0)
			ft_fprintf(2, ERROR_NO_SUCH_FILE, expanded_arg + 8);
		cmd->data->last_exit_status = 1;
		cmd->has_error = 1;
		free(expanded_arg);
		return (1);
	}
	return (0);
}
