/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_parentheses_bonus.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 16:00:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * Check if parentheses are balanced in the input
 * @param argv: Array of arguments
 * @param argc: Number of arguments
 * @return: 1 if balanced, 0 if not
 */
static int	ft_check_parentheses_balance(char **argv, int argc)
{
	int	i;
	int	j;
	int	balance;
	int	in_quotes;
	char	quote_char;

	balance = 0;
	i = 0;
	while (i < argc)
	{
		j = 0;
		in_quotes = 0;
		quote_char = 0;
		while (argv[i][j])
		{
			if (!in_quotes && (argv[i][j] == '\'' || argv[i][j] == '"'))
			{
				in_quotes = 1;
				quote_char = argv[i][j];
			}
			else if (in_quotes && argv[i][j] == quote_char)
				in_quotes = 0;
			else if (!in_quotes)
			{
				if (argv[i][j] == '(')
					balance++;
				else if (argv[i][j] == ')')
					balance--;
			}
			j++;
		}
		i++;
	}
	return (balance == 0);
}

/**
 * Find matching closing parenthesis
 * @param argv: Array of arguments
 * @param argc: Number of arguments
 * @param start: Starting position (should be at opening parenthesis)
 * @return: Index of matching closing parenthesis, or -1 if not found
 */
static int	ft_find_matching_paren(char **argv, int argc, int start)
{
	int	i;
	int	j;
	int	balance;
	int	in_quotes;
	char	quote_char;

	balance = 0;
	i = start;
	while (i < argc)
	{
		j = 0;
		in_quotes = 0;
		quote_char = 0;
		while (argv[i][j])
		{
			if (!in_quotes && (argv[i][j] == '\'' || argv[i][j] == '"'))
			{
				in_quotes = 1;
				quote_char = argv[i][j];
			}
			else if (in_quotes && argv[i][j] == quote_char)
				in_quotes = 0;
			else if (!in_quotes)
			{
				if (argv[i][j] == '(')
					balance++;
				else if (argv[i][j] == ')')
				{
					balance--;
					if (balance == 0)
						return (i);
				}
			}
			j++;
		}
		i++;
	}
	return (-1);
}

/**
 * Extract arguments from a parenthesized expression
 * @param argv: Array of arguments
 * @param start: Start index (after opening parenthesis)
 * @param end: End index (before closing parenthesis)
 * @param new_argc: Pointer to store new argument count
 * @return: New array of arguments from inside parentheses
 */
static char	**ft_extract_paren_args(char **argv, int start, int end, int *new_argc)
{
	char	**new_argv;
	int		i;
	int		j;
	char	*temp;
	char	*cleaned;

	*new_argc = end - start + 1;
	new_argv = malloc(sizeof(char *) * (*new_argc + 1));
	if (!new_argv)
		return (NULL);
	
	i = start;
	j = 0;
	while (i <= end)
	{
		temp = argv[i];
		if (i == start && temp[0] == '(')
		{
			cleaned = ft_strdup(temp + 1);
			if (cleaned && ft_strlen(cleaned) > 0)
				new_argv[j++] = cleaned;
			else if (cleaned)
				free(cleaned);
		}
		else if (i == end && temp[ft_strlen(temp) - 1] == ')')
		{
			cleaned = ft_substr(temp, 0, ft_strlen(temp) - 1);
			if (cleaned && ft_strlen(cleaned) > 0)
				new_argv[j++] = cleaned;
			else if (cleaned)
				free(cleaned);
		}
		else
			new_argv[j++] = ft_strdup(temp);
		i++;
	}
	new_argv[j] = NULL;
	*new_argc = j;
	return (new_argv);
}

/**
 * Execute a parenthesized expression
 * @param argv: Array of arguments
 * @param start: Start index of parentheses
 * @param end: End index of parentheses
 * @param data: Shell data structure
 * @return: Exit status of the expression
 */
static int	ft_execute_paren_expression(char **argv, int start, int end, t_data *data)
{
	char	**paren_argv;
	int		paren_argc;
	t_cmd	*paren_cmd_list;
	int		exit_status;
	pid_t	*pids;

	paren_argv = ft_extract_paren_args(argv, start, end, &paren_argc);
	if (!paren_argv)
		return (1);
	
	data->argc = paren_argc;
	paren_cmd_list = ft_parse_input(paren_argv, data);
	if (!paren_cmd_list)
	{
		ft_free_matrix(paren_argv);
		return (1);
	}
	
	pids = malloc(sizeof(pid_t) * data->cmd_count);
	if (!pids)
	{
		ft_free_matrix(paren_argv);
		return (1);
	}
	
	exit_status = ft_execute_logical_chain(paren_cmd_list, &data, pids);
	
	free(pids);
	ft_free_matrix(paren_argv);
	// TODO: Free paren_cmd_list properly
	
	return (exit_status);
}

/**
 * Handle parentheses in command parsing and execution
 * @param argv: Array of arguments
 * @param data: Shell data structure
 * @return: 1 if parentheses were found and handled, 0 if none found
 */
int	ft_handle_parentheses(char **argv, t_data *data)
{
	int	i;
	int	paren_start;
	int	paren_end;
	int	has_parentheses;

	if (!argv || !data)
		return (0);
	
	if (!ft_check_parentheses_balance(argv, data->argc))
	{
		ft_fprintf(2, "minishell: syntax error: unmatched parentheses\n");
		data->last_exit_status = 2;
		return (1);
	}
	
	has_parentheses = 0;
	i = 0;
	while (i < data->argc)
	{
		if (ft_strchr(argv[i], '('))
		{
			has_parentheses = 1;
			paren_start = i;
			paren_end = ft_find_matching_paren(argv, data->argc, i);
			if (paren_end == -1)
			{
				ft_fprintf(2, "minishell: syntax error: unmatched parentheses\n");
				data->last_exit_status = 2;
				return (1);
			}
			
			data->last_exit_status = ft_execute_paren_expression(argv, paren_start, paren_end, data);
			i = paren_end + 1;
		}
		else
			i++;
	}
	
	return (has_parentheses);
}
