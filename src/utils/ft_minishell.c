/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/04 15:33:44 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include "../minishell.h"

// Yo creo que deberíamos hacer un split del input
int	ft_process_input(char *input)
{
	// Process the input command
	printf("Processing input: %s\n", input);
	return (0);
}

int	ft_minishell(void)
{
	char *input;

	ft_msg_start();
	while (1)
	{
		input = readline("\033[90mminishell> \033[0m");
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			if (ft_process_input(input))
				return (free(input), -1);
		}
		free(input);
	}
	clear_history();
	return (0);
}
