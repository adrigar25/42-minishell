/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_heredoc_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 12:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 22:27:16 by agarcia          ###   ########.fr       */
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
	size_t	len;
	ssize_t	nread;

	line = NULL;
	len = 0;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(1, HEREDOC_PROMPT, 9);
		line = NULL;
		nread = getline(&line, &len, stdin);
		if (nread == -1 || !line)
			break ;
		if (nread > 0 && line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_write_heredoc_line(write_fd, line, nread);
		free(line);
	}
	return (0);
}

/**
 * ENGLISH: Handles the heredoc functionality by creating a pipe,
 * reading input until the delimiter is found,
 * and returning the read end of the pipe.
 *
 * SPANISH: Maneja la funcionalidad heredoc creando un pipe, leyendo
 * la entrada hasta encontrar el delimitador,
 * y devolviendo el descriptor de lectura del pipe.
 *
 * @param delimiter   The delimiter string to end heredoc input. /
 *                    La cadena delimitadora para finalizar la entrada
 * 					del heredoc.
 *
 * @returns The file descriptor for reading heredoc content, or -1 on error. /
 *          El descriptor de archivo para leer el contenido del heredoc,
 * 			o -1 en caso de error.
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
