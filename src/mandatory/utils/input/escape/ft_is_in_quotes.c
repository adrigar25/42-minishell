/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_in_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auto <auto@local>                           +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:00:00 by auto              #+#    #+#             */
/*   Updated: 2025/11/16 22:00:00 by auto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_is_in_quotes(const char *input, int pos)
{
	int		i;
	int		in_q;
	char	q;

	i = 0;
	in_q = 0;
	q = 0;
	while (i <= pos && input[i])
	{
		if (!in_q && (input[i] == '\'' || input[i] == '"'))
		{
			in_q = 1;
			q = input[i];
		}
		else if (in_q && input[i] == q)
		{
			in_q = 0;
			q = 0;
		}
		i++;
	}
	return (in_q);
}
