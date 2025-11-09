/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_execute_cmds_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:32:13 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 14:10:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * ENGLISH: Executes a single built-in command directly in the parent process
 *          if it meets the criteria (no pipes, no redirections, no errors).
 *
 * SPANISH: Ejecuta un comando interno único directamente en el proceso padre
 *          si cumple con los criterios (sin tuberías, sin redirecciones,
 *          sin errores).
 *
 * @param cmd   The command to execute. /
 *              El comando a ejecutar.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the built-in was executed, 0 otherwise. /
 *          1 si se ejecutó el interno, 0 en caso contrario.
 */
static int	ft_execute_single_builtin(t_cmd *cmd, t_data **data)
{
	if (!ft_is_builtin(cmd))
		return (0);
	if (cmd->infd != STDIN_FILENO || cmd->outfd != STDOUT_FILENO)
		return (0);
	if (cmd->next || cmd->has_error)
		return (0);
	(*data)->last_exit_status = ft_exec_builtin(cmd, data);
	return (1);
}

/**
 * ENGLISH: Forks a new process to execute a command, setting up its I/O
 *          and handling built-in or external commands.
 *
 * SPANISH: Crea un nuevo proceso para ejecutar un comando, configurando su
 *          entrada/salida y manejando comandos internos o externos.
 *
 * @param cmd       The command to execute. /
 *                  El comando a ejecutar.
 *
 * @param cmd_list  The head of the command list for I/O setup. /
 *                  La cabeza de la lista de comandos para la configuración
 *                  de E/S.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @param pids      Array to store the PIDs of forked processes. /
 *                  Arreglo para almacenar los PIDs de los procesos creados.
 *
 * @returns 0 on success, -1 on fork error. /
 *          0 en caso de éxito, -1 en caso de error al crear el proceso.
 */
static int	ft_fork_cmd(t_cmd *cmd, t_cmd *cmd_list, t_data **data, pid_t *pids)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		ft_setup_child_io(cmd, cmd_list);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (cmd->has_error)
			(*data)->last_exit_status = 1;
		else if (ft_is_builtin(cmd))
			(*data)->last_exit_status = ft_exec_builtin(cmd, data);
		else
			(*data)->last_exit_status = ft_exec_bin(cmd);
		exit((*data)->last_exit_status);
	}
	if (pid > 0)
	{
		pids[cmd->index] = pid;
		if (cmd->infd != STDIN_FILENO)
			close(cmd->infd);
		if (cmd->outfd != STDOUT_FILENO)
			close(cmd->outfd);
		return (0);
	}
	return (perror("fork"), -1);
}

/**
 * ENGLISH: Waits for the last command in a pipeline to finish and updates
 *          the shell's last exit status.
 *
 * SPANISH: Espera a que el último comando en una tubería termine y actualiza
 *          el último estado de salida del shell.
 *
 * @param status    Pointer to store the exit status. /
 *                  Puntero para almacenar el estado de salida.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @param pids      Array of PIDs of forked processes. /
 *                  Arreglo de PIDs de los procesos creados.
 *
 * @param idx       Index of the last command in the pipeline. /
 *                  Índice del último comando en la tubería.
 */
static void	ft_wait_last(int *status, t_data **data, pid_t *pids, int idx)
{
	if (pids[idx] <= 0)
		return ;
	waitpid(pids[idx], status, 0);
	if (WIFEXITED(*status))
		(*data)->last_exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		(*data)->last_exit_status = 128 + WTERMSIG(*status);
	pids[idx] = 0;
}

/**
 * ENGLISH: Executes a list of commands, handling built-ins directly when
 *          possible, forking for others, and managing pipelines.
 *
 * SPANISH: Ejecuta una lista de comandos, manejando los internos directamente
 *          cuando es posible, creando procesos para otros y gestionando
 *          tuberías.
 *
 * @param cmd_list  The head of the command list to execute. /
 *                  La cabeza de la lista de comandos a ejecutar.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @returns The last exit status of the executed commands, or -1 on error. /
 *          El último estado de salida de los comandos ejecutados, o -1
 *          en caso de error.
 */
static int	ft_exec_cmd(t_cmd *cmd, t_cmd *cmd_list, t_data **data, pid_t *pids)
{
	if (ft_should_execute(&cmd, *data))
		return (0);
	if (ft_execute_single_builtin(cmd, data))
		return (1);
	if (ft_fork_cmd(cmd, cmd_list, data, pids) == -1)
		return (-1);
	return (0);
}

/**
 * ENGLISH: Executes a list of commands, handling built-ins directly when
 *          possible, forking for others, and managing pipelines.
 *
 * SPANISH: Ejecuta una lista de comandos, manejando los internos directamente
 *          cuando es posible, creando procesos para otros y gestionando
 *          tuberías.
 *
 * @param cmd_list  The head of the command list to execute. /
 *                  La cabeza de la lista de comandos a ejecutar.
 *
 * @param data      Pointer to the shell data structure. /
 *                  Puntero a la estructura de datos del shell.
 *
 * @returns The last exit status of the executed commands, or -1 on error. /
 *          El último estado de salida de los comandos ejecutados, o -1
 *          en caso de error.
 */
int	ft_execute_cmds(t_cmd *cmd_list, t_data **data)
{
	t_cmd	*cmd;
	pid_t	*pids;
	int		status;

	if (!cmd_list || !data || !*data)
		return (-1);
	pids = calloc((*data)->cmd_count, sizeof(pid_t));
	if (!pids)
		return (-1);
	cmd = cmd_list;
	while (cmd)
	{
		if (ft_exec_cmd(cmd, cmd_list, data, pids) == -1)
			return (ft_finish_execution(pids, cmd_list, *data));
		if (!cmd->next || cmd->next->op != OP_PIPE)
			ft_wait_last(&status, data, pids, cmd->index);
		cmd = cmd->next;
	}
	ft_finish_execution(pids, cmd_list, *data);
	return ((*data)->last_exit_status);
}
