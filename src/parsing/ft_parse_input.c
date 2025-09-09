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
	printf("DEBUG: ft_add_fd_to_cmd called with fd=%d, in_or_out=%d\n", fd,
		in_or_out);
	printf("DEBUG: Before - infd=%d, outfd=%d\n", cmd->infd, cmd->outfd);
	if (in_or_out == 0) // Input redirection
	{
		if (cmd->infd != STDIN_FD)
			close(cmd->infd);
		cmd->infd = fd;
		printf("DEBUG: Set infd to %d\n", fd);
	}
	else // Output redirection (in_or_out == 1)
	{
		if (cmd->outfd != STDOUT_FD)
			close(cmd->outfd);
		cmd->outfd = fd;
		printf("DEBUG: Set outfd to %d\n", fd);
	}
	printf("DEBUG: After - infd=%d, outfd=%d\n", cmd->infd, cmd->outfd);
}

static t_cmd	*ft_create_cmd_node(void)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->argv = NULL;
	new_cmd->infd = STDIN_FD;   // Inicializar con stdin
	new_cmd->outfd = STDOUT_FD; // Inicializar con stdout
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
	new_argv[i] = arg;      // Agregar el nuevo argumento
	new_argv[i + 1] = NULL; // Terminar con NULL
	free(cmd->argv);
	cmd->argv = new_argv;
}

t_cmd	*ft_parse_input(char **argv, int argc)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	int		i;
	int		fd;
	char	*arg;
	char	*clean_arg;
	int		pipefd[2];

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
			// Crear pipe entre comando actual y siguiente
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				return (cmd_list);
			}
			// El comando actual escribe al pipe
			if (current_cmd->outfd == STDOUT_FD)
				// Solo si no hay redirección de salida
				current_cmd->outfd = pipefd[1];
			else
				close(pipefd[1]); // Cerrar write end si ya hay redirección
			// Crear siguiente comando
			current_cmd->next = ft_create_cmd_node();
			current_cmd = current_cmd->next;
			// El siguiente comando lee del pipe
			current_cmd->infd = pipefd[0];
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
			i++;
		}
		else
		{
			arg = argv[i];
			clean_arg = ft_trim(arg, ' ');
			if (clean_arg)
				ft_add_arg_to_cmd(current_cmd, clean_arg);
		}
		i++;
	}
	return (cmd_list);
}
