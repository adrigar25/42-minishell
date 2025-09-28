/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_should_execute.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 16:19:59 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 16:23:06 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	ft_should_execute(t_cmd **current, t_data *data)
{
	if (!current || !data)
		return (0);
	if (((*current)->op == OP_OR && data->last_exit_status == EXIT_SUCCESS)
		|| ((*current)->op == OP_AND && data->last_exit_status != EXIT_SUCCESS))
	{
		*current = (*current)->next;
		return (1);
	}
	return (0);
}
