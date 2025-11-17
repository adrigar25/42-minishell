/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_input_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 21:44:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Reads input from the user, displaying a prompt if in interactive
 * 			mode.
 *          Adds non-empty input to history if in interactive mode.
 *
 * SPANISH: Lee la entrada del usuario, mostrando un aviso si está en modo
 * 			interactivo.
 *          Añade la entrada no vacía al historial si está en modo interactivo.
 *
 * @param input  Pointer to store the input string. /
 *               Puntero para almacenar la cadena de entrada.
 *
 * @param data   Pointer to the data structure containing environment and status
 * 				info. /
 *               Puntero a la estructura de datos que contiene información del
 * 				entorno y estado.
 *
 * @returns 1 if input was read successfully, 0 if EOF was encountered. /
 *          1 si se leyó la entrada con éxito, 0 si se encontró EOF.
 */
int	ft_read_input(char **input, t_data *data)
{
	char	*prompt;

	// size_t	len;
	while (1)
	{
		if (data->isatty)
		{
			prompt = ft_generate_prompt(data->envp);
			*input = readline(prompt);
			free(prompt);
		}
		else
		{
			*input = readline("minishell> ");
		}
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
