/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:22:57 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:22:17 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Searches for a file with the given filename in the specified
 * 			directory.
 *          If the directory is NULL, it uses the current working directory
 * 			(PWD).
 *
 * SPANISH: Busca un archivo con el nombre dado en el directorio especificado.
 *          Si el directorio es NULL, utiliza el directorio de
 * 			trabajo actual (PWD).
 *
 * @param dir       The directory to search in, or NULL to use PWD. /
 *                  El directorio donde buscar, o NULL para usar PWD.
 *
 * @param filename  The name of the file to search for. /
 *                  El nombre del archivo a buscar.
 *
 * @returns The full path to the file if found, or NULL if not found. /
 *          La ruta completa al archivo si se encuentra, o NULL si
 * 			no se encuentra.
 */
char	*ft_search_file(const char *dir, const char *filename)
{
	const char	*start_dir;

	if (dir)
		start_dir = dir;
	else
		start_dir = getenv("PWD");
	return (ft_search_in_dir(start_dir, filename));
}
