/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_exit_status.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:05:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_expand_exit_status(char **dst, int *j, t_data *data)
{
	char	*temp;

	temp = ft_itoa(data->last_exit_status);
	if (!temp)
		return (0);
	if (!ft_append(dst, temp))
	{
		free(temp);
		return (0);
	}
	free(temp);
	(*j) += 2;
	return (1);
}
