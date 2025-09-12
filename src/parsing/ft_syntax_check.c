/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:25:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 17:24:03 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_syntax_errors(char **argv, int argc)
{
	int		i;
	char	*error_msg;
	char	*temp;

	if (!argv || argc == 0)
		return (0);
	if (ft_strcmp(argv[0], "|") == 0)
	{
		ft_putstr_error("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	if (ft_strcmp(argv[0], ">") == 0 || ft_strcmp(argv[0], ">>") == 0
		|| ft_strcmp(argv[0], "<") == 0 || ft_strcmp(argv[0], "<<") == 0)
	{
		ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
		return (1);
	}
	i = 0;
	while (i < argc)
	{
		if ((ft_strcmp(argv[i], ">") == 0 || ft_strcmp(argv[i], ">>") == 0
				|| ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i],
					"<<") == 0) && i + 1 >= argc)
		{
			ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
			return (1);
		}
		if ((ft_strcmp(argv[i], ">") == 0 || ft_strcmp(argv[i], ">>") == 0
				|| ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i],
					"<<") == 0) && i + 1 < argc)
		{
			if (ft_strcmp(argv[i + 1], "|") == 0 || ft_strcmp(argv[i + 1],
					">") == 0 || ft_strcmp(argv[i + 1], ">>") == 0
				|| ft_strcmp(argv[i + 1], "<") == 0 || ft_strcmp(argv[i + 1],
					"<<") == 0)
			{
				temp = ft_strjoin(argv[i + 1], "'\n");
				error_msg = ft_strjoin("minishell: syntax error near unexpected token `",
						temp);
				ft_putstr_error(error_msg);
				free(temp);
				free(error_msg);
				return (1);
			}
		}
		if (ft_strcmp(argv[i], "|") == 0 && i + 1 < argc)
		{
			if (ft_strcmp(argv[i + 1], "|") == 0)
			{
				ft_putstr_error("minishell: syntax error near unexpected token `|'\n");
				return (1);
			}
			if (ft_strcmp(argv[i + 1], ">") == 0 || ft_strcmp(argv[i + 1],
					">>") == 0)
			{
				ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
				return (1);
			}
		}
		if (ft_strcmp(argv[i], "|") == 0 && i + 1 >= argc)
		{
			ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
			return (1);
		}
		if (ft_strcmp(argv[i], ">>") == 0 && i + 1 < argc && ft_strcmp(argv[i
				+ 1], ">") == 0)
		{
			ft_putstr_error("minishell: syntax error near unexpected token `>'\n");
			return (1);
		}
		i++;
	}
	return (0);
}
