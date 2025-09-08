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

static t_cmd	*ft_create_cmd_node(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->infds = malloc(1 * sizeof(int));
	new_cmd->outfds = malloc(1 * sizeof(int));
	if (!new_cmd->infds || !new_cmd->outfds)
	{
		free(new_cmd->infds);
		free(new_cmd->outfds);
		free(new_cmd);
		return (NULL);
	}
	new_cmd->infds[0] = STDIN_FD;
	new_cmd->infds[1] = -1;
	new_cmd->outfds[0] = STDOUT_FD;
	new_cmd->outfds[1] = -1;
	new_cmd->next = NULL;
	return (new_cmd);
}

static void	ft_add_fd_to_cmd(t_cmd *cmd, int fd, int in_or_out)
{
	int	*new_fds;
	int	count;
	int	i;

	if (!cmd)
		return ;
	count = 0;
	if (in_or_out == 0)
	{
		if (cmd->infds[0] == STDIN_FD)
		{
			cmd->infds[0] = fd;
			return ;
		}
		while (cmd->infds[count] != -1)
			count++;
		new_fds = malloc((count + 2) * sizeof(int));
		if (!new_fds)
			return ;
		i = 0;
		while (i < count)
		{
			new_fds[i] = cmd->infds[i];
			i++;
		}
		new_fds[count] = fd;
		new_fds[count + 1] = -1;
		free(cmd->infds);
		cmd->infds = new_fds;
	}
	else
	{
		if (cmd->outfds[0] == STDOUT_FD)
		{
			cmd->outfds[0] = fd;
			return ;
		}
		while (cmd->outfds[count] != -1 && count < 10)
			count++;
		new_fds = malloc((count + 2) * sizeof(int));
		if (!new_fds)
			return ;
		i = 0;
		while (i < count)
		{
			new_fds[i] = cmd->outfds[i];
			i++;
		}
		new_fds[count] = fd;
		new_fds[count + 1] = -1;
		free(cmd->outfds);
		cmd->outfds = new_fds;
	}
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
	new_argv[i++] = arg;
	new_argv[i] = NULL;
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*ft_parse_input(char **argv, int argc)
{
	t_cmd *cmd_list;
	t_cmd *current_cmd;
	int i;

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
			current_cmd->next = ft_create_cmd_node();
			current_cmd = current_cmd->next;
		}
		else if (ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
			|| ft_strcmp(argv[i], ">>") == 0)
		{
			if (i + 1 >= argc)
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				return (cmd_list);
			}
			if (ft_strcmp(argv[i + 1], "|") == 0 || ft_strcmp(argv[i + 1],
					"<") == 0 || ft_strcmp(argv[i + 1], ">") == 0
				|| ft_strcmp(argv[i + 1], ">>") == 0)
			{
				printf("minishell: syntax error near unexpected token `%s'\n",
					argv[i + 1]);
				return (cmd_list);
			}
			// Pasar el argumento completo (sin modificar) a las funciones de manejo de archivos
			if (ft_strcmp(argv[i], "<") == 0)
			{
				int fd = ft_handle_infile(argv[i + 1]);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 0);
			}
			else if (ft_strcmp(argv[i], ">") == 0)
			{
				int fd = ft_handle_outfile(argv[i + 1], 0);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 1);
			}
			else if (ft_strcmp(argv[i], ">>") == 0)
			{
				int fd = ft_handle_outfile(argv[i + 1], 1);
				if (fd != -1)
					ft_add_fd_to_cmd(current_cmd, fd, 1);
			}
			i++;
		}
		else
		{
			// Remove leading spaces from the argument (but keep escapes as-is)
			char *arg = argv[i];
			while (*arg == ' ')
				arg++;
			// Duplicate the trimmed argument to ensure ownership
			char *clean_arg = strdup(arg);
			if (clean_arg)
				ft_add_arg_to_cmd(current_cmd, clean_arg);
		}

		i++;
	}

	return (cmd_list);
}