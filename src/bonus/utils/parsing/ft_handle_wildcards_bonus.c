/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 14:16:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <dirent.h>
#include <sys/stat.h>

/**
 * Check if a pattern matches a filename using wildcards
 * @param pattern: The pattern with wildcards (e.g., "*.c")
 * @param filename: The filename to match against
 * @return: 1 if matches, 0 if not
 */
static int	ft_match_pattern(const char *pattern, const char *filename)
{
	const char	*p;
	const char	*f;
	const char	*star_pattern;
	const char	*star_filename;

	p = pattern;
	f = filename;
	star_pattern = NULL;
	star_filename = NULL;
	while (*f)
	{
		if (*p == '*')
		{
			star_pattern = ++p;
			star_filename = f;
		}
		else if (*p == *f)
		{
			p++;
			f++;
		}
		else if (star_pattern)
		{
			p = star_pattern;
			f = ++star_filename;
		}
		else
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}

/**
 * Check if a string contains wildcards
 * @param str: String to check
 * @return: 1 if contains wildcards, 0 if not
 */
static int	ft_has_wildcards(const char *str)
{
	int	i;
	int	in_quotes;

	i = 0;
	in_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			in_quotes = !in_quotes;
		else if (!in_quotes && str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

/**
 * Count matching files in current directory
 * @param pattern: The wildcard pattern
 * @return: Number of matching files
 */
static int	ft_count_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && ft_match_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

/**
 * Expand wildcards in a single argument
 * @param pattern: The wildcard pattern
 * @param matches: Array to store matching filenames
 * @param max_matches: Maximum number of matches to store
 * @return: Number of matches found
 */
static int	ft_expand_wildcard(const char *pattern, char **matches,
		int max_matches)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry && count < max_matches)
	{
		if (entry->d_name[0] != '.' && ft_match_pattern(pattern, entry->d_name))
		{
			matches[count] = ft_strdup(entry->d_name);
			if (!matches[count])
			{
				closedir(dir);
				return (count);
			}
			count++;
		}
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

/**
 * Sort array of strings alphabetically
 * @param arr: Array of strings to sort
 * @param size: Size of the array
 */
static void	ft_sort_strings(char **arr, int size)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < size - 1)
	{
		j = i + 1;
		while (j < size)
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
			{
				temp = arr[i];
				arr[i] = arr[j];
				arr[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/**
 * Handle wildcard expansion for all arguments
 * @param argv: Array of arguments that may contain wildcards
 * @param data: Shell data structure
 * @return: New array with wildcards expanded, or original on error
 */
char	**ft_handle_wildcards(char **argv, t_data *data)
{
	int		i;
	int		j;
	int		total_args;
	int		matches;
	char	**new_argv;
	char	**temp_matches;
	int		new_argc;

	if (!argv || !data)
		return (argv);
	// Contar total de argumentos después de expansión
	total_args = 0;
	i = 0;
	while (argv[i])
	{
		if (i > 0)
		{
			if (ft_strcmp(argv[i - 1], "<") == 0 || ft_strcmp(argv[i - 1],
					">") == 0 || ft_strcmp(argv[i - 1], ">>") == 0)
			{
				if (ft_has_wildcards(argv[i]))
				{
					ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, argv[i]);
					data->argc = 0;
					return (NULL);
				}
				total_args++;
				i++;
				continue ;
			}
			else if (ft_strcmp(argv[i - 1], "<<") == 0)
			{
				total_args++;
				i++;
				continue ;
			}
		}
		if (ft_has_wildcards(argv[i]))
		{
			matches = ft_count_matches(argv[i]);
			total_args += (matches > 0) ? matches : 1;
		}
		else
			total_args++;
		i++;
	}
	new_argv = malloc(sizeof(char *) * (total_args + 1));
	if (!new_argv)
		return (argv);
	new_argc = 0;
	i = 0;
	while (argv[i])
	{
		if (i > 0 && ft_strcmp(argv[i - 1], "<<") == 0)
		{
			new_argv[new_argc++] = ft_strdup(argv[i]);
			i++;
			continue ;
		}
		if (i > 0 && (ft_strcmp(argv[i - 1], "<") == 0 || ft_strcmp(argv[i - 1],
					">") == 0 || ft_strcmp(argv[i - 1], ">>") == 0))
		{
			new_argv[new_argc++] = ft_strdup(argv[i]);
			i++;
			continue ;
		}
		if (ft_has_wildcards(argv[i]))
		{
			matches = ft_count_matches(argv[i]);
			if (matches > 0)
			{
				temp_matches = malloc(sizeof(char *) * matches);
				if (!temp_matches)
				{
					ft_free_matrix_size(new_argv, new_argc);
					return (argv);
				}
				matches = ft_expand_wildcard(argv[i], temp_matches, matches);
				ft_sort_strings(temp_matches, matches);
				for (j = 0; j < matches; j++)
					new_argv[new_argc++] = temp_matches[j];
				free(temp_matches);
			}
			else
				new_argv[new_argc++] = ft_strdup(argv[i]);
		}
		else
			new_argv[new_argc++] = ft_strdup(argv[i]);
		i++;
	}
	new_argv[new_argc] = NULL;
	data->argc = new_argc;
	return (new_argv);
}
