/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 20:09:39 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_read_input(char **input, t_data *data)
{
	char	*prompt;

	while (1)
	{
		if (data->isatty)
		{
			prompt = ft_generate_prompt(data->envp);
			*input = readline(prompt);
			free(prompt);
		}
		else
			*input = readline("minishell> ");
		if (!*input)
			return (0);
		if (**input)
		{
			if (data->isatty)
				add_history(*input);
			return (1);
		}
		free(*input);
	}
}
