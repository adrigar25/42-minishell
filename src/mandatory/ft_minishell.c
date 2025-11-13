/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 17:47:21 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/13 17:47:56 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_data(t_data **data)
{
	*data = malloc(sizeof(t_data));
	if (!*data)
		return (1);
	(*data)->last_exit_status = 0;
	(*data)->envp = NULL;
	(*data)->isatty = 0;
	return (0);
}

static int	init_envp(t_data **data, char **envp)
{
	(*data)->envp = ft_dupenv(envp);
	if (!(*data)->envp)
	{
		free(*data);
		return (1);
	}
	return (0);
}

static void	update_shlvl(t_data *data)
{
	char	*val;
	int		lvl;
	char	*newlvl;

	val = ft_getenv("SHLVL", data->envp);
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
		if (ft_getenv("SHLVL", data->envp))
			ft_update_existing_env("SHLVL", newlvl, data->envp);
		else
			ft_setenv("SHLVL", newlvl, &(data->envp));
		free(newlvl);
	}
}

static int	ft_init_data(t_data **data, char **envp)
{
	if (alloc_data(data))
		return (1);
	if (init_envp(data, envp))
		return (1);
	(*data)->isatty = isatty(STDIN_FILENO);
	update_shlvl(*data);
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
	ft_cleanup(data);
	return (exit_status);
}
