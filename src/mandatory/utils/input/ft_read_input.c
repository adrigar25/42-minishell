/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/20 18:04:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*read_interactive(t_data *data)
{
	char	*prompt;
	char	*input;

	prompt = ft_generate_prompt(data->envp);
	input = readline(prompt);
	free(prompt);
	return (input);
}

static char	*read_noninteractive(void)
{
	char	*input;
	size_t	len;

	input = ft_get_next_line(STDIN_FILENO);
	if (input)
	{
		len = ft_strlen(input);
		if (len > 0 && input[len - 1] == '\n')
			input[len - 1] = '\0';
	}
	return (input);
}

int	ft_read_input(char **input, t_data *data)
{
	while (1)
	{
		if (data->isatty)
			*input = read_interactive(data);
		else
			*input = read_noninteractive();
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
