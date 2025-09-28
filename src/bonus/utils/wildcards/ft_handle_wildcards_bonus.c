/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_wildcards_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:36:44 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:35:41 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Handles wildcard expansion in the given argument array.
 *          It expands arguments containing wildcards into matching filenames
 *          from the current directory, while preserving heredoc delimiters.
 *
 * SPANISH: Maneja la expansión de comodines en el arreglo de argumentos dado.
 *          Expande los argumentos que contienen comodines en nombres de archivos
 *          coincidentes del directorio actual, preservando los delimitadores
 *          heredoc.
 *
 * @param argv  The original argument array. /
 *              El arreglo de argumentos original.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns A new argument array with wildcards expanded, or the original array
 *          on failure. The caller is responsible for freeing the returned
 *          array. /
 *          Un nuevo arreglo de argumentos con los comodines expandidos,
 *          o el arreglo original en caso de fallo. El llamador es responsable
 *          de liberar el arreglo retornado.
 */
static int	count_total_args(char **argv)
{
	int	total_args;
	int	i;
	int	matches;

	total_args = 0;
	i = 0;
	while (argv[i])
	{
		if (ft_has_wildcards(argv[i]))
		{
			matches = ft_count_matches(argv[i]);
			if (matches > 0)
				total_args += matches;
			else
				total_args += 1;
		}
		else
			total_args++;
		i++;
	}
	return (total_args);
}

/**
 * ENGLISH: Copies the heredoc delimiter argument to the new argument array
 *          without expanding it.
 *
 * SPANISH: Copia el argumento delimitador heredoc al nuevo arreglo de argumentos
 *          sin expandirlo.
 *
 * @param argv      The original argument array. /
 *                  El arreglo de argumentos original.
 *
 * @param new_argv  The new argument array being constructed. /
 *                  El nuevo arreglo de argumentos que se está construyendo.
 *
 * @param i         Pointer to the current index in the original array. /
 *                  Puntero al índice actual en el arreglo original.
 *
 * @param new_argc  Pointer to the current count of arguments in the new array. /
 *                  Puntero al conteo actual de argumentos en el nuevo arreglo.
 *
 * @returns 0 on success, -1 on memory allocation failure. /
 *          0 en caso de éxito, -1 en caso de fallo de asignación de memoria.
 *
 * 		(Always returns 0 in current implementation.)
 */
static int	copy_heredoc_arg(char **argv, char **new_argv, int *i,
		int *new_argc)
{
	new_argv[(*new_argc)++] = ft_strdup(argv[*i]);
	(*i)++;
	return (0);
}

/**
 * ENGLISH: Processes a single argument containing wildcards,
 *          expanding it into matching filenames and adding them to the new
 *          array.
 *
 * SPANISH: Procesa un solo argumento que contiene comodines,
 *          expandiéndolo en nombres de archivos coincidentes y añadiéndolos
 *          al nuevo arreglo.
 *
 * @param arg       The argument containing wildcards. /
 *                  El argumento que contiene comodines.
 *
 * @param new_argv  The new argument array being constructed. /
 *                  El nuevo arreglo de argumentos que se está construyendo.
 *
 * @param new_argc  Pointer to the current count of arguments in the new array. /
 *                  Puntero al conteo actual de argumentos en el nuevo arreglo.
 *
 * @returns 0 on success, -1 on memory allocation failure. /
 *          0 en caso de éxito, -1 en caso de fallo de asignación de memoria.
 */
static int	ft_process_wildcard(char *arg, char **new_argv, int *new_argc)
{
	int		matches;
	char	**temp_matches;
	int		k;

	matches = ft_count_matches(arg);
	if (matches > 0)
	{
		temp_matches = malloc(sizeof(char *) * matches);
		if (!temp_matches)
			return (-1);
		matches = ft_expand_wildcard(arg, temp_matches, matches);
		ft_sort_strings(temp_matches, matches);
		k = 0;
		while (k < matches)
		{
			new_argv[(*new_argc)++] = temp_matches[k];
			k++;
		}
		free(temp_matches);
	}
	else
		new_argv[(*new_argc)++] = ft_strdup(arg);
	return (0);
}

/**
 * ENGLISH: Expands and copies arguments from the original array to the new
 *          array, handling wildcards and heredoc delimiters.
 *
 * SPANISH: Expande y copia argumentos del arreglo original al nuevo arreglo,
 *          manejando comodines y delimitadores heredoc.
 *
 * @param argv      The original argument array. /
 *                  El arreglo de argumentos original.
 *
 * @param new_argv  The new argument array being constructed. /
 *                  El nuevo arreglo de argumentos que se está construyendo.
 *
 * @param data      The data structure containing relevant information. /
 *                  La estructura de datos que contiene información relevante.
 *
 * @returns 0 on success, -1 on memory allocation failure. /
 *          0 en caso de éxito, -1 en caso de fallo de asignación de memoria.
 */
static int	expand_and_copy_args(char **argv, char **new_argv, t_data *data)
{
	int	i;
	int	new_argc;

	i = 0;
	new_argc = 0;
	while (argv[i])
	{
		if (i > 0 && ft_strcmp(argv[i - 1], "<<") == 0)
			if (copy_heredoc_arg(argv, new_argv, &i, &new_argc) == -1)
				return (-1);
		if (ft_has_wildcards(argv[i]))
		{
			if (ft_process_wildcard(argv[i], new_argv, &new_argc) == -1)
			{
				ft_free_matrix_size(new_argv, new_argc);
				return (-1);
			}
		}
		else
			new_argv[new_argc++] = ft_strdup(argv[i]);
		i++;
	}
	new_argv[new_argc] = NULL;
	data->argc = new_argc;
	return (0);
}

/**
 * ENGLISH: Handles wildcard expansion in the given argument array.
 *          It expands arguments containing wildcards into matching filenames
 *          from the current directory, while preserving heredoc delimiters.
 *
 * SPANISH: Maneja la expansión de comodines en el arreglo de argumentos dado.
 *          Expande los argumentos que contienen comodines en nombres de archivos
 *          coincidentes del directorio actual, preservando los delimitadores
 *          heredoc.
 *
 * @param argv  The original argument array. /
 *              El arreglo de argumentos original.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns A new argument array with wildcards expanded, or the original array
 *          on failure. The caller is responsible for freeing the returned
 *          array. /
 *          Un nuevo arreglo de argumentos con los comodines expandidos,
 *          o el arreglo original en caso de fallo. El llamador es responsable
 *          de liberar el arreglo retornado.
 */
char	**ft_handle_wildcards(char **argv, t_data *data)
{
	int		total_args;
	char	**new_argv;

	if (!argv || !data)
		return (argv);
	total_args = count_total_args(argv);
	new_argv = malloc(sizeof(char *) * (total_args + 1));
	if (!new_argv)
		return (argv);
	if (expand_and_copy_args(argv, new_argv, data) == -1)
		return (argv);
	return (new_argv);
}
