/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_wildcards_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:36:44 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 14:02:40 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <stdlib.h>

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

static int	copy_heredoc_arg(char **argv, char **new_argv, int *i,
		int *new_argc)
{
	new_argv[(*new_argc)++] = ft_strdup(argv[*i]);
	(*i)++;
	return (0);
}

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
