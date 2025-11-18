/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_match_pattern_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:46:11 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/18 01:29:59 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	handle_star(const char **pat, const char **str,
		const char **last_star, const char **str_backup)
{
	*last_star = (*pat)++;
	*str_backup = *str;
	return (1);
}

static int	handle_backtrack(const char **pat, const char **str,
		const char *last_star, const char **str_backup)
{
	if (!last_star)
		return (0);
	*pat = last_star + 1;
	*str = ++(*str_backup);
	return (1);
}

int	ft_match_pattern(const char *pattern, const char *filename)
{
	const char	*pat;
	const char	*str;
	const char	*last_star;
	const char	*str_backup;

	pat = pattern;
	str = filename;
	last_star = NULL;
	str_backup = NULL;
	while (*str)
	{
		if (*pat == '?' || *pat == *str)
		{
			pat++;
			str++;
		}
		else if (*pat == '*')
			handle_star(&pat, &str, &last_star, &str_backup);
		else if (!handle_backtrack(&pat, &str, last_star, &str_backup))
			return (0);
	}
	while (*pat == '*')
		pat++;
	return (*pat == '\0');
}
