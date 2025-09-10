/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/11 00:01:04 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>
#include <signal.h>
#include <unistd.h>

static int	g_heredoc_interrupted = 0;

static int	ft_isalnum_custom(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9'));
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	write(STDOUT_FD, "<", 1);
	close(STDIN_FD);
}

static int	ft_write_to_pipe(int write_fd, char *line)
{
	if (write(write_fd, line, ft_strlen(line)) == -1)
		return (-1);
	if (write(write_fd, "
", 1) == -1)
		return (-1);
	return (0);
}

static char	*ft_expand_heredoc_vars(char *line, t_data *data)
{
	char	*result;
	char	*temp;
	char	*old_result;
	int		i;
	int		start;
	char	*var_name;
	char	*var_value;

	if (!line || !data)
		return (line);
	result = ft_strdup("");
	if (!result)
		return (line);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			i++;
			if (line[i] == '?')
			{
				temp = ft_itoa(data->last_exit_status);
				if (temp)
				{
					old_result = result;
					result = ft_strjoin(result, temp);
					free(old_result);
					free(temp);
				}
				i++;
			}
			else if (ft_isalnum_custom(line[i]) || line[i] == '_')
			{
				start = i;
				while (line[i] && (ft_isalnum_custom(line[i])
						|| line[i] == '_'))
					i++;
				var_name = ft_substr(line, start, i - start);
				if (var_name)
				{
					var_value = ft_getenv(var_name, data->envp);
					if (var_value)
					{
						old_result = result;
						result = ft_strjoin(result, var_value);
						free(old_result);
					}
					free(var_name);
				}
			}
			else
			{
				temp = ft_substr(line, i - 1, 2);
				if (temp)
				{
					old_result = result;
					result = ft_strjoin(result, temp);
					free(old_result);
					free(temp);
				}
				i++;
			}
		}
		else
		{
			temp = ft_substr(line, i, 1);
			if (temp)
			{
				old_result = result;
				result = ft_strjoin(result, temp);
				free(old_result);
				free(temp);
			}
			i++;
		}
	}
	free(line);
	return (result);
}

int	ft_handle_heredoc(char *delimiter, t_data *data)
{
	int					pipefd[2];
	char				*line;
	struct sigaction	sa;
	struct sigaction	old_sa;

	if (!delimiter)
	{
		ft_putstr_error(ERROR_HEREDOC_DELIMITER);
		return (-1);
	}
	if (pipe(pipefd) == -1)
	{
		ft_putstr_error(ERROR_HEREDOC_PROCESS);
		return (-1);
	}
	// Setup signal handler for Ctrl+C
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	g_heredoc_interrupted = 0;
	while (1)
	{
		line = readline(HEREDOC_PROMPT);
		if (g_heredoc_interrupted)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			sigaction(SIGINT, &old_sa, NULL);
			return (-1);
		}
		if (!line) // EOF (Ctrl+D)
		{
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Expand variables in heredoc
		line = ft_expand_heredoc_vars(line, data);
		if (ft_write_to_pipe(pipefd[1], line) == -1)
		{
			free(line);
			close(pipefd[0]);
			close(pipefd[1]);
			sigaction(SIGINT, &old_sa, NULL);
			ft_putstr_error(ERROR_HEREDOC_PROCESS);
			return (-1);
		}
		free(line);
	}
	close(pipefd[1]);
	sigaction(SIGINT, &old_sa, NULL);
	return (pipefd[0]);
}

#include "../minishell.h"
#include <ctype.h>
#include <signal.h>
#include <unistd.h>

static int	g_heredoc_interrupted = 0;

static int	ft_isalnum_custom(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0'
			&& c <= '9'));
}

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	g_heredoc_interrupted = 1;
	write(STDOUT_FD, "\n", 1);
	close(STDIN_FD);
}

static int	ft_write_to_pipe(int write_fd, char *line)
{
	if (write(write_fd, line, ft_strlen(line)) == -1)
		return (-1);
	if (write(write_fd, "\n", 1) == -1)
		return (-1);
	return (0);
}

static char	*ft_expand_heredoc_vars(char *line, t_data *data)
{
	char	*result;
	char	*temp;
	int		i;
	int		start;
	char	*var_name;
	char	*var_value;
	char	*old_result;
	char	*old_result;
	char	*old_result;
	char	*old_result;

	if (!line || !data)
		return (line);
	result = ft_strdup("");
	if (!result)
		return (line);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			i++;
			if (line[i] == '?')
			{
				temp = ft_itoa(data->last_exit_status);
				if (temp)
				{
					old_result = result;
					result = ft_strjoin(result, temp);
					free(old_result);
					free(temp);
				}
				i++;
			}
			else if (ft_isalnum_custom(line[i]) || line[i] == '_')
			{
				start = i;
				while (line[i] && (ft_isalnum_custom(line[i])
						|| line[i] == '_'))
					i++;
				var_name = ft_substr(line, start, i - start);
				if (var_name)
				{
					var_value = ft_getenv(var_name, data->envp);
					if (var_value)
					{
						old_result = result;
						result = ft_strjoin(result, var_value);
						free(old_result);
					}
					free(var_name);
				}
			}
			else
			{
				temp = ft_substr(line, i - 1, 2);
				if (temp)
				{
					old_result = result;
					result = ft_strjoin(result, temp);
					free(old_result);
					free(temp);
				}
				i++;
			}
		}
		else
		{
			temp = ft_substr(line, i, 1);
			if (temp)
			{
				old_result = result;
				result = ft_strjoin(result, temp);
				free(old_result);
				free(temp);
			}
			i++;
		}
	}
	free(line);
	return (result);
}

int	ft_handle_heredoc(char *delimiter, t_data *data)
{
	int					pipefd[2];
	char				*line;
	struct sigaction	sa;
	struct sigaction	old_sa;

	if (!delimiter)
	{
		ft_putstr_error(ERROR_HEREDOC_DELIMITER);
		return (-1);
	}
	if (pipe(pipefd) == -1)
	{
		ft_putstr_error(ERROR_HEREDOC_PROCESS);
		return (-1);
	}
	sa.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	g_heredoc_interrupted = 0;
	while (1)
	{
		line = readline(HEREDOC_PROMPT);
		if (g_heredoc_interrupted)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			sigaction(SIGINT, &old_sa, NULL);
			return (-1);
		}
		if (!line) // EOF (Ctrl+D)
		{
			break ;
		}
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// Expand variables in heredoc
		line = ft_expand_heredoc_vars(line, data);
		if (ft_write_to_pipe(pipefd[1], line) == -1)
		{
			free(line);
			close(pipefd[0]);
			close(pipefd[1]);
			sigaction(SIGINT, &old_sa, NULL);
			ft_putstr_error(ERROR_HEREDOC_PROCESS);
			return (-1);
		}
		free(line);
	}
	close(pipefd[1]);
	sigaction(SIGINT, &old_sa, NULL);
	return (pipefd[0]);
}
