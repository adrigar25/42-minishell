/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_wildcards.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 20:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 20:22:41 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
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
static int	ft_expand_wildcard(const char *pattern, char **matches, int max_matches)
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
	// Count total arguments needed after expansion
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
				total_args++; // Keep original if no matches
		}
		else
			total_args++;
		i++;
	}
	// Allocate new array
	new_argv = malloc(sizeof(char *) * (total_args + 1));
	if (!new_argv)
		return (argv);
	// Expand wildcards
	new_argc = 0;
	i = 0;
	while (argv[i])
	{
		if (ft_has_wildcards(argv[i]))
		{
			matches = ft_count_matches(argv[i]);
			if (matches > 0)
			{
				temp_matches = malloc(sizeof(char *) * matches);
				if (!temp_matches)
				{
					ft_free_char_array_size(new_argv, new_argc);
					return (argv);
				}
				matches = ft_expand_wildcard(argv[i], temp_matches, matches);
				ft_sort_strings(temp_matches, matches);
				j = 0;
				while (j < matches)
				{
					new_argv[new_argc] = temp_matches[j];
					new_argc++;
					j++;
				}
				free(temp_matches);
			}
			else
			{
				// No matches found, keep original
				new_argv[new_argc] = ft_strdup(argv[i]);
				if (!new_argv[new_argc])
				{
					ft_free_char_array_size(new_argv, new_argc);
					return (argv);
				}
				new_argc++;
			}
		}
		else
		{
			// No wildcards, copy as is
			new_argv[new_argc] = ft_strdup(argv[i]);
			if (!new_argv[new_argc])
			{
				ft_free_char_array_size(new_argv, new_argc);
				return (argv);
			}
			new_argc++;
		}
		i++;
	}
	new_argv[new_argc] = NULL;
	// Update data->argc with new count
	data->argc = new_argc;
	return (new_argv);
}
