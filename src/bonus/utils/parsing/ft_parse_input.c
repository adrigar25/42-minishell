/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:06:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/08 18:08:08 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_redir_token(const char *s)
{
	return (ft_strcmp(s, "<") == 0 || ft_strcmp(s, ">") == 0 || ft_strcmp(s,
			">>") == 0 || ft_strcmp(s, "<<") == 0);
}

static int	is_pipe_like_token(const char *s)
{
	return (ft_strcmp(s, "|") == 0 || ft_strcmp(s, "||") == 0 || ft_strcmp(s,
			"&&") == 0 || ft_strcmp(s, "&") == 0);
}

static void	ft_add_fd_to_cmd(t_cmd *cmd, int fd, int in_or_out)
{
	if (!cmd)
		return ;
	if (in_or_out == 0)
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

static t_cmd	*ft_create_cmd_node(int index)
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
	new_argv[i] = arg;
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*ft_parse_input(char **argv, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		fd;
	char	*arg;
	char	*clean_arg;
	int		pipefd[2];
	int		cmd_index;
	int		k;
	int		count_nonop;
	t_cmd	*prev_cmd;

	if (!argv || data->argc == 0)
		return (NULL);
	cmd_index = 0;
	cmd_list = ft_create_cmd_node(cmd_index);
	if (!cmd_list)
		return (NULL);
	current_cmd = cmd_list;
	current_cmd->data = data;
	current_cmd->op = 0;
	i = 0;
	while (i < data->argc)
	{
		if (is_pipe_like_token(argv[i]))
		{
			cmd_index++;
			prev_cmd = current_cmd;
			current_cmd->next = ft_create_cmd_node(cmd_index);
			current_cmd = current_cmd->next;
			current_cmd->data = data;
			if (!ft_strcmp(argv[i], "|") && ft_strlen(argv[i]) == 1)
			{
				if (pipe(pipefd) == -1)
				{
					perror("pipe");
					return (cmd_list);
				}
				if (prev_cmd->outfd == STDOUT_FILENO)
					prev_cmd->outfd = pipefd[1];
				else
					close(pipefd[1]);
				current_cmd->infd = pipefd[0];
				current_cmd->op = 1;
			}
			else if (!ft_strcmp(argv[i], "||"))
				current_cmd->op = 2;
			else if (!ft_strcmp(argv[i], "&&"))
				current_cmd->op = 3;
		}
		else if ((ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
				|| ft_strcmp(argv[i], ">>") == 0 || ft_strcmp(argv[i],
					"<<") == 0) && current_cmd->has_error == 0)
		{
			k = i + 1;
			count_nonop = 0;
			while (k < data->argc && !is_pipe_like_token(argv[k])
				&& !is_redir_token(argv[k]))
			{
				count_nonop++;
				k++;
			}
			if (count_nonop > 1)
			{
				ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, argv[i + 1]);
				data->last_exit_status = 1;
				current_cmd->has_error = 1;
				i = k - 1;
				i++;
				continue ;
			}
			clean_arg = ft_remove_quotes(argv[i + 1]);
			if (!clean_arg)
				clean_arg = argv[i + 1];
			if (ft_strcmp(argv[i], "<") == 0)
				fd = ft_handle_infile(clean_arg);
			else if (ft_strcmp(argv[i], ">") == 0)
				fd = ft_handle_outfile(clean_arg, 0);
			else if (ft_strcmp(argv[i], ">>") == 0)
				fd = ft_handle_outfile(clean_arg, 1);
			else
				fd = ft_handle_heredoc(clean_arg);
			if (fd != -1)
			{
				ft_add_fd_to_cmd(current_cmd, fd, (ft_strcmp(argv[i], "<") == 0
						|| ft_strcmp(argv[i], "<<") == 0 ? 0 : 1));
			}
			else
			{
				data->last_exit_status = 1;
				current_cmd->has_error = 1;
			}
			if (clean_arg != argv[i + 1])
				free(clean_arg);
			i++;
		}
		else
		{
			arg = argv[i];
			clean_arg = ft_remove_quotes(arg);
			if (clean_arg)
				ft_add_arg_to_cmd(current_cmd, clean_arg);
		}
		i++;
	}
	return (cmd_list);
}
