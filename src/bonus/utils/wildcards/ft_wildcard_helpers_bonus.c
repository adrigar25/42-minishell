/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wildcard_helpers_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 01:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 01:30:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	is_prev_redir(char **a, int idx)
{
	if (idx == 0)
		return (0);
	if (ft_strcmp(a[idx - 1], "<") == 0)
		return (1);
	if (ft_strcmp(a[idx - 1], ">") == 0)
		return (1);
	if (ft_strcmp(a[idx - 1], ">>") == 0)
		return (1);
	return (0);
}

int	is_prev_heredoc(char **a, int idx)
{
	if (idx == 0)
		return (0);
	if (ft_strcmp(a[idx - 1], "<<") == 0)
		return (1);
	return (0);
}

int	count_total_args(char **argv)
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

int	ft_process_wildcard(char *arg, char **new_argv, int *new_argc)
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
