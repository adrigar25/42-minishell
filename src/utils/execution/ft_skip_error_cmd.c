/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_error_cmd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42madrid>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:45:00 by agarcia           #+#    #+#             */
/*   Updated: 2024/12/19 10:45:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_skip_error_cmd(t_cmd *cmd_list, t_data *data, pid_t *pids)
{
	data->last_exit_status = 1;
	pids[cmd_list->index] = -1;
	return (1);
}
