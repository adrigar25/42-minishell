/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_match_pattern_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 01:46:11 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 02:40:38 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <dirent.h>
#include <sys/stat.h>

static void	ft_advance_pattern(const char **p, const char **f,
		const char **star_pattern, const char **star_filename)
{
	if (**p == '*')
	{
		while (**p == '*')
			(*p)++;
		if (**p == '\0')
			return ;
		*star_pattern = *p;
		*star_filename = *f;
	}
	else if (**p == '?' || **p == **f)
	{
		(*p)++;
		(*f)++;
	}
	else if (*star_pattern)
	{
		*p = *star_pattern;
		*f = ++(*star_filename);
	}
}

int	ft_match_pattern(const char *pattern, const char *filename)
{
	const char	*p = pattern;
	const char	*f = filename;
	const char	*star_pattern = NULL;
	const char	*star_filename = NULL;

	while (*f)
	{
		if (*p == '*' || *p == '?' || *p == *f || star_pattern)
			ft_advance_pattern(&p, &f, &star_pattern, &star_filename);
		else
			return (0);
	}
	while (*p == '*')
		p++;
	return (*p == '\0');
}
