/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/10/30 01:25:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * ENGLISH: Initializes the shell data structure with environment variables.
 *
 * SPANISH: Inicializa la estructura de datos del shell con las variables
 * 			de entorno.
 *
 * @param data   Pointer to the shell data structure to initialize. /
 *               Puntero a la estructura de datos del shell a inicializar.
 *
 * @param envp   The environment variables. /
 *               Las variables de entorno.
 *
 * @returns 0 on success, 1 on failure. /
 *          0 en caso de éxito, 1 en caso de error.
 */
static int	ft_init_data(t_data **data, char **envp)
{
		char *val;
		int lvl;
		char *newlvl;

	*data = malloc(sizeof(t_data));
	if (!*data)
		return (1);
	(*data)->last_exit_status = 0;
	(*data)->envp = ft_dupenv(envp);
	if (!(*data)->envp)
	{
		free(*data);
		return (1);
	}
	(*data)->isatty = isatty(STDIN_FILENO);
	/* Update SHLVL for this shell instance */
	{
		val = ft_getenv("SHLVL", (*data)->envp);
		if (val && ft_is_number(val))
			lvl = ft_atoi(val);
		else
			lvl = 0;
		if (lvl < 0)
			lvl = 0;
		else if (lvl >= 1000)
			lvl = 1;
		else
			lvl = lvl + 1;
		newlvl = ft_itoa(lvl);
		if (newlvl)
		{
			if (ft_getenv("SHLVL", (*data)->envp))
				ft_update_existing_env("SHLVL", newlvl, (*data)->envp);
			else
				ft_setenv("SHLVL", newlvl, &((*data)->envp));
			free(newlvl);
		}
	}
	return (0);
}

/**
 * ENGLISH: The main function for the minishell program.
 *          It initializes the shell, displays a welcome message if in
 *          interactive mode,
 *          reads user input, processes commands, and executes
 *          them in a loop until exit.
 *
 * SPANISH: La función principal para el programa minishell.
 *          Inicializa el shell, muestra un mensaje de bienvenida si está
 *          en modo interactivo,
 *          lee la entrada del usuario, procesa los comandos y los ejecuta
 *          en un bucle hasta salir.
 *
 * @param envp   The environment variables. /
 *               Las variables de entorno.
 *
 * @param debug  Flag to indicate if debug information should be shown. /
 *               Indicador para indicar si se debe mostrar información de
 *               depuración.
 *
 * @returns The exit status of the last executed command. /
 *          El estado de salida del último comando ejecutado.
 */
int	ft_minishell(char **envp, int debug)
{
	char	*input;
	t_cmd	*cmd_list;
	t_data	*data;
	int		exit_status;

	if (ft_init_data(&data, envp))
		return (1);
	if (data->isatty)
		ft_msg_start();
	ft_init_signals();
	exit_status = 0;
	while (ft_read_input(&input, data))
	{
		cmd_list = ft_process_input(input, data, debug);
		exit_status = ft_execute_cmds(cmd_list, &data);
	}
	ft_free_matrix(data->envp);
	free(data);
	return (exit_status);
}
