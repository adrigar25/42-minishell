/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_in_subdirs_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:24:06 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/27 22:27:16 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Searches for a file with the given filename
 * 		in subdirectories of the specified directory.
 *
 * SPANISH: Busca un archivo con el nombre dado en los
 * 		subdirectorios del directorio especificado.
 *
 * @param dir       The directory to search in. /
 *                  El directorio donde buscar.
 *
 * @param filename  The name of the file to search for. /
 *                  El nombre del archivo a buscar.
 *
 * @returns The full path to the file if found, or NULL if not found. /
 *          La ruta completa al archivo si se encuentra,
	o NULL si no se encuentra.
 */
static char	*ft_search_in_subdir_utils(const char *dir, const char *entry_name,
		const char *filename)
{
	char	*sub_path;
	char	*result;
	DIR		*subdir;

	result = NULL;
	sub_path = ft_build_path(dir, entry_name);
	subdir = opendir(sub_path);
	if (subdir)
	{
		closedir(subdir);
		result = ft_search_file(sub_path, filename);
	}
	free(sub_path);
	return (result);
}

/**
 * ENGLISH: Searches for a file with the given filename
 * 		in all entries (subdirectories) of the specified directory.
 *
 * SPANISH: Busca un archivo con el nombre dado en todas las
 * 		entradas (subdirectorios) del directorio especificado.
 *
 * @param d         Pointer to the opened directory stream. /
 *                  Puntero al flujo de directorio abierto.
 *
 * @param dir       The directory to search in. /
 *                  El directorio donde buscar.
 *
 * @param filename  The name of the file to search for. /
 *                  El nombre del archivo a buscar.
 *
 * @returns The full path to the file if found, or NULL if not found. /
 *          La ruta completa al archivo si se encuentra,
	o NULL si no se encuentra.
 */
static char	*ft_search_entries(DIR *d, const char *dir, const char *filename)
{
	struct dirent	*entry;
	char			*result;
	int				dot_status;

	result = NULL;
	entry = readdir(d);
	while (entry)
	{
		if (ft_is_dot_or_dotdot(entry->d_name))
		{
			entry = readdir(d);
			continue ;
		}
		result = ft_search_in_subdir_utils(dir, entry->d_name, filename);
		if (result)
			break ;
		entry = readdir(d);
	}
	return (result);
}

/**
 * ENGLISH: Searches for a file with the given filename in all
 * 		subdirectories of the specified directory.
 *
 * SPANISH: Busca un archivo con el nombre dado en todos los
 * 		subdirectorios del directorio especificado.
 *
 * @param dir       The directory to search in. /
 *                  El directorio donde buscar.
 *
 * @param filename  The name of the file to search for. /
 *                  El nombre del archivo a buscar.
 *
 * @returns The full path to the file if found, or NULL if not found. /
 *          La ruta completa al archivo si se encuentra,
	o NULL si no se encuentra.
 */
char	*ft_search_in_subdirs(const char *dir, const char *filename)
{
	DIR		*d;
	char	*result;

	result = NULL;
	d = opendir(dir);
	if (!d)
		return (NULL);
	result = ft_search_entries(d, dir, filename);
	closedir(d);
	return (result);
}
