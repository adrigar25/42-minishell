/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/04 17:46:23 by adriescr         ###   ########.fr       */
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
	int		count_args;
	char	*input;
	char	*prompt;

	ft_msg_start();
	while (1)
	{
		prompt = ft_strjoin(ft_strjoin("\033[90mminishell:(", ft_get_directory_path(prompt)), ")\033[0m> ");
		input = readline(prompt);
		if (!input)
			break ;
		if (input && *input)
		{
			count_args = ft_count_args(input);
			printf("Number of arguments: %d\n", count_args);
			add_history(input);
			if (ft_process_input(input))
				return (free(input), -1);
		}
		free(input);
	}
	clear_history();
	return (0);
}