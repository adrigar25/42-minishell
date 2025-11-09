/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 18:05:39 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <stdio.h>
#include <termios.h>

/**
 * ENGLISH: Processes a single line read from stdin for the heredoc.
 *          Compares it to the delimiter and writes it to the heredoc
 *          file descriptor if it doesn't match.
 *
 * SPANISH: Procesa una sola línea leída desde stdin para el heredoc.
 *          La compara con el delimitador y la escribe en el descriptor
 *          de archivo heredoc si no coincide.
 *
 * @param write_fd   The file descriptor to write heredoc lines to. /
 *                   El descriptor de archivo donde escribir las líneas del
 *                   heredoc.
 *
 * @param line       The line read from stdin. /
 *                   La línea leída desde stdin.
 *
 * @param delimiter  The delimiter string to end heredoc input. /
 *                   La cadena delimitadora para finalizar la entrada del
 *                   heredoc.
 *
 * @returns 1 if the line matches the delimiter (end of heredoc), 0 otherwise. /
 *          1 si la línea coincide con el delimitador (fin del heredoc), 0 en
 *          caso contrario.
 */
static int	ft_process_heredoc_line(int write_fd, char *line,
		const char *delimiter, t_data *data, int expand)
{
	char	*cmp;
	size_t	nread;
	char	*expanded;

	nread = ft_strlen(line);
	if (nread > 0 && line[nread - 1] == '\n')
		cmp = ft_substr(line, 0, nread - 1);
	else
		cmp = ft_strdup(line);
	if (!cmp)
	{
		free(line);
		return (-1);
	}
	if (ft_strcmp(cmp, delimiter) == 0)
	{
		free(cmp);
		free(line);
		return (1);
	}
	free(cmp);
	if (expand)
	{
		expanded = NULL;
		if (!ft_process_arg(&expanded, line, data))
		{
			free(line);
			return (-1);
		}
		if (expanded)
		{
			write(write_fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(write_fd, line, nread);
	free(line);
	return (0);
}

/**
 * ENGLISH: Reads lines from stdin until the delimiter is found, writing each
 * 			line to the heredoc file descriptor.
 *
 * SPANISH: Lee líneas desde stdin hasta encontrar el delimitador, escribiendo
 * 			cada línea en el descriptor de archivo heredoc.
 *
 * @param write_fd   The file descriptor to write heredoc lines to. /
 *                   El descriptor de archivo donde escribir las líneas del
 *                   heredoc.
 *
 * @param delimiter  The delimiter string to end heredoc input. /
 *                   La cadena delimitadora para finalizar la entrada del
 *                   heredoc.
 *
 * @returns 0 on success. /
 *          0 en caso de éxito.
 */
static int	ft_read_heredoc_loop(int write_fd, const char *delimiter,
	t_data *data, int expand)
{
    char	*line;

	/* Use ft_get_next_line for heredoc input even in interactive mode to
	   avoid readline state conflicts between processes. Print the heredoc
	   prompt manually when running in a tty to preserve user feedback. */
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, HEREDOC_PROMPT, ft_strlen(HEREDOC_PROMPT));
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_process_heredoc_line(write_fd, line, delimiter, data,
				expand) != 0)
			break ;
	}
	return (0);
}

/**
	* ENGLISH: Handles the heredoc functionality by creating a pipe,
	* reading input until the delimiter is found,
	* and returning the read end of the pipe.
	*
	* SPANISH: Maneja la funcionalidad de heredoc creando una tubería,
	* leyendo la entrada hasta que se encuentra el delimitador,
	* y devolviendo el extremo de lectura de la tubería.
	*
	* @param delimiter  The delimiter string to end heredoc input. /
	*                   La cadena delimitadora para finalizar la entrada del
	*                   heredoc.
	*
	* @returns The read end of the pipe on success, -1 on failure. /
	*          El extremo de lectura de la tubería en caso de éxito,
		-1 en caso de
	*          fallo.
	*/

int	ft_heredoc(const char *delimiter, t_data *data, int expand)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (-1);
	/* If interactive (tty), fork a child to read heredoc so signals are
		handled cleanly by the child. For non-tty (scripted) input, read
		heredoc in the current process to avoid losing buffered stdin data. */
	if (data && data->isatty)
	{
		struct termios orig_term;
		if (tcgetattr(STDIN_FILENO, &orig_term) == -1)
			; /* non-fatal: continue without restoring if it fails */
		/* Ignore SIGINT in parent while child reads heredoc so only the
		   heredoc child receives Ctrl+C and parent doesn't print prompt
		   via the sigint handler (avoids duplicate main prompt). */
		signal(SIGINT, SIG_IGN);
		pid = fork();
		if (pid < 0)
		{
			close(pipefd[0]);
			close(pipefd[1]);
			return (-1);
		}
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			close(pipefd[0]);
			ft_read_heredoc_loop(pipefd[1], delimiter, data, expand);
			close(pipefd[1]);
			exit(0);
		}
		close(pipefd[1]);
		if (waitpid(pid, &status, 0) == -1)
		{
			close(pipefd[0]);
			/* restore parent handler */
			signal(SIGINT, sigint_handler);
			return (-1);
		}
		/* restore parent handler */
		signal(SIGINT, sigint_handler);
		/* Restore terminal attributes in the parent. Ignore errors. */
		tcsetattr(STDIN_FILENO, TCSANOW, &orig_term);
		/* Reset readline internal state */
		rl_on_new_line();
		rl_replace_line("", 0);
		/* If child was terminated by SIGINT, print newline so prompt
		   appears on a fresh line (we ignored SIGINT in parent). */
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			close(pipefd[0]);
			write(1, "\n", 1);
			/* Indicate heredoc was aborted by SIGINT with a special code */
			return (-2);
		}
		return (pipefd[0]);
	}
	/* non-tty: read heredoc inline so parent's buffered input is consumed
		in order and heredoc lines are not lost */
	ft_read_heredoc_loop(pipefd[1], delimiter, data, expand);
	close(pipefd[1]);
	return (pipefd[0]);
}
