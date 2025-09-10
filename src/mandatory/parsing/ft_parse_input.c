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

#include "../minishell.h"

static void	ft_add_fd_to_cmd(t_cmd *cmd, int fd, int in_or_out)
{
	if (!cmd)
		return ;
	if (in_or_out == 0)
	{
		if (cmd->infd != STDIN_FD)
			close(cmd->infd);
		cmd->infd = fd;
	}
	else
	{
		if (cmd->outfd != STDOUT_FD)
			close(cmd->outfd);
		cmd->outfd = fd;
	}
}

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
	new_argv[i] = arg;
	new_argv[i + 1] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*ft_parse_input(char **argv, int argc, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		fd;
	char	*arg;
	char	*clean_arg;
	int		pipefd[2];
	char	*error_msg;

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
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return (cmd_list);
			}
			if (current_cmd->outfd == STDOUT_FD)
				current_cmd->outfd = pipefd[1];
			else
				close(pipefd[1]);
			current_cmd->next = ft_create_cmd_node();
			current_cmd = current_cmd->next;
			current_cmd->infd = pipefd[0];
		}
		else if (ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
			|| ft_strcmp(argv[i], ">>") == 0 || ft_strcmp(argv[i], "<<") == 0)
		{
			if (i + 1 >= argc)
			{
				ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
				return (cmd_list);
			}
			if (ft_strcmp(argv[i + 1], "|") == 0 || ft_strcmp(argv[i + 1],
					"<") == 0 || ft_strcmp(argv[i + 1], ">") == 0
				|| ft_strcmp(argv[i + 1], ">>") == 0 || ft_strcmp(argv[i + 1],
					"<<") == 0)
			{
				error_msg = ft_strjoin(ft_strjoin("minishell: syntax error near unexpected token `",
							argv[i + 1]), "'\n");
				ft_putstr_error(error_msg);
				free(error_msg);
				return (cmd_list);
			}
			if (ft_strcmp(argv[i], "<") == 0)
			{
				fd = ft_handle_infile(argv[i + 1]);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 0);
			}
			else if (ft_strcmp(argv[i], ">") == 0)
			{
				fd = ft_handle_outfile(argv[i + 1], 0);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 1);
			}
			else if (ft_strcmp(argv[i], ">>") == 0)
			{
				fd = ft_handle_outfile(argv[i + 1], 1);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 1);
			}
			else if (ft_strcmp(argv[i], "<<") == 0)
			{
				fd = ft_handle_heredoc(argv[i + 1], data);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 0);
			}
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
