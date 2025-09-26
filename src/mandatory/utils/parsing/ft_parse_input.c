/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:06:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/26 22:08:18 by adriescr         ###   ########.fr       */
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

static int	ft_handle_redirection(t_cmd *cmd, char **argv, int i, t_data *data)
{
	char	*clean_arg;
	int		fd;

	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	if (ft_strcmp(argv[i], "<") == 0)
		fd = ft_handle_infile(clean_arg);
	else if (ft_strcmp(argv[i], ">") == 0 || ft_strcmp(argv[i], ">>") == 0)
		fd = ft_handle_outfile(clean_arg, ft_strcmp(argv[i], ">>") == 0);
	else
		fd = ft_handle_heredoc(clean_arg);
	if (fd != -1)
	{
		if (ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], "<<") == 0)
		{
			if (cmd->infd != STDIN_FILENO)
				close(cmd->infd);
			cmd->infd = fd;
		}
		else
		{
			if (cmd->outfd != STDOUT_FILENO)
				close(cmd->outfd);
			cmd->outfd = fd;
		}
	}
	else
	{
		data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (i + 1);
}

static int	ft_process_token(t_cmd **current_cmd, char **argv, int i,
		int *cmd_index, t_data *data)
{
	int		pipefd[2];
	t_cmd	*new_cmd;
	char	*clean_arg;

	if (ft_strcmp(argv[i], "|") == 0)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			return (i);
		}
		new_cmd = ft_create_cmd_node(*cmd_index + 1);
		if (!new_cmd)
			return (i);
		(*cmd_index)++;
		(*current_cmd)->next = new_cmd;
		new_cmd->data = data;
		if ((*current_cmd)->outfd == STDOUT_FILENO)
			(*current_cmd)->outfd = pipefd[1];
		else
			close(pipefd[1]);
		new_cmd->infd = pipefd[0];
		*current_cmd = new_cmd;
		return (i);
	}
	else if ((ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
			|| ft_strcmp(argv[i], ">>") == 0 || ft_strcmp(argv[i], "<<") == 0)
		&& (*current_cmd)->has_error == 0)
		return (ft_handle_redirection(*current_cmd, argv, i, data));
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (clean_arg)
			ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i);
	}
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
