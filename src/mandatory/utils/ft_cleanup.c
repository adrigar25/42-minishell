/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cleanup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:46:58 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/13 17:53:05 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_cleanup(t_data *data)
{
	if (data)
	{
		if (data->envp)
			ft_free_matrix(data->envp);
		free(data);
	}
	clear_history();
	rl_clear_history();
}
