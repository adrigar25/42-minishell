/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:37:42 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 16:26:05 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Libera el array de arrays de comandos
static void	free_split_strings(char ***cmds)
{
	int	i;
	int	j;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i][j])
		{
			free(cmds[i][j]);
			j++;
		}
		free(cmds[i]);
		i++;
	}
	free(cmds);
}

// Divide argv en un array de comandos (char ***), separando por sep
char	***ft_split_strings(const char **argv, const char *sep)
{
	int		count;
	char	***cmds;
	int		i;
	int		start;
	int		end;
	int		j;
	int		k;

	count = 0;
	i = 0;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], sep) == 0)
			i++;
		else
		{
			count++;
			while (argv[i] && ft_strcmp(argv[i], sep) != 0)
				i++;
		}
	}
	cmds = malloc((count + 1) * sizeof(char **));
	if (!cmds)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], sep) == 0)
		{
			i++;
			continue ;
		}
		start = i;
		while (argv[i] && ft_strcmp(argv[i], sep) != 0)
			i++;
		end = i;
		cmds[j] = malloc((end - start + 1) * sizeof(char *));
		if (!cmds[j])
		{
			free_split_strings(cmds);
			return (NULL);
		}
		k = 0;
		while (start < end)
		{
			cmds[j][k] = ft_strdup(argv[start]);
			if (!cmds[j][k])
			{
				free_split_strings(cmds);
				return (NULL);
			}
			k++;
			start++;
		}
		cmds[j][k] = NULL;
		j++;
	}
	cmds[j] = NULL;
	return (cmds);
}

int	ft_pipex(const char **argv, char **envp)
{
	int		pipefds[2];
	char	***cmds;
	int		i;
	pid_t	pid;
	int		prev_fd;
	int		in_fd;
	int		out_fd;

	prev_fd = -1;
	i = 0;
	cmds = ft_split_strings(argv, "|");
	if (!cmds)
		return (1);
	while (cmds && cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefds) == -1)
		{
			perror("pipe");
			free_split_strings(cmds);
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free_split_strings(cmds);
			return (1);
		}
		if (pid == 0)
		{
			if (prev_fd == -1)
				in_fd = 0;
			else
				in_fd = prev_fd;
			if (cmds[i + 1])
				out_fd = pipefds[1];
			else
				out_fd = 1;
			ft_exec_cmd(cmds[i], in_fd, out_fd, envp);
			exit(1);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1])
		{
			close(pipefds[1]);
			prev_fd = pipefds[0];
		}
		i++;
	}
	while (i-- > 0)
		wait(NULL);
	free_split_strings(cmds);
	return (0);
}
