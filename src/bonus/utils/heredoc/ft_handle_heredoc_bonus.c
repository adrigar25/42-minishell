/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 15:55:33 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Writes a line to the heredoc file descriptor, ensuring
 * 			it ends with a newline.
 *
 * SPANISH: Escribe una línea en el descriptor de archivo heredoc,
 * 			asegurando que termine con un salto de línea.
 *
 * @param fd     The file descriptor to write to. /
 *               El descriptor de archivo donde escribir.
 *
 * @param line   The line to write. /
 *               La línea a escribir.
 *
 * @param nread  The number of bytes read from the line. /
 *               El número de bytes leídos de la línea.
 *
 * @returns 0 on success. /
 *          0 en caso de éxito.
 */
static int	ft_write_heredoc_line(int fd, char *line, ssize_t nread)
{
	if (nread > 0 && line[nread - 1] == '\n')
		line[nread - 1] = '\n';
	else
		line[nread] = '\n';
	write(fd, line, nread);
	return (0);
}

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
		const char *delimiter)
{
	char	*cmp;
	size_t	nread;

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
	write(write_fd, line, nread);
	if (nread > 0 && line[nread - 1] == '\n')
		write(write_fd, "\n", 1);
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
static int	ft_read_heredoc_loop(int write_fd, const char *delimiter)
{
	char	*line;
	int		ret;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, HEREDOC_PROMPT, 9);
		line = ft_get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		ret = ft_process_heredoc_line(write_fd, line, delimiter);
		if (ret != 0)
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
 *          El extremo de lectura de la tubería en caso de éxito, -1 en caso de
 *          fallo.
 */

int	ft_handle_heredoc(const char *delimiter)
{
	int	pipefd[2];

	if (pipe(pipefd) == -1)
		return (-1);
	ft_read_heredoc_loop(pipefd[1], delimiter);
	close(pipefd[1]);
	return (pipefd[0]);
}
