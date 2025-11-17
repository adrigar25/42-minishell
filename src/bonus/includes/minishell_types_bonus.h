/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types_bonus.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 23:55:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 22:40:09 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_BONUS_H
# define MINISHELL_TYPES_BONUS_H

/**
 * ENGLISH: Represents the main structure for managing the shell state.
 *
 * SPANISH: Representa la estructura principal para gestionar el estado del
 * 	shell.
 */
typedef struct s_data
{
	char			**argv;
	char			**envp;
	int				argc;
	int				cmd_count;
	int				isatty;
	int				last_exit_status;
	struct s_data	*next;
}					t_data;

/**
 * ENGLISH: Represents a command node in the minishell command list.
 *
 * SPANISH: Representa un nodo de comando en la lista de comandos de minishell.
 */
typedef struct s_cmd
{
	char			**argv;
	int				infd;
	int				outfd;
	int				has_error;
	int				index;
	int				op;
	t_data			*data;
	struct s_cmd	*next;
}					t_cmd;

#endif /* MINISHELL_TYPES_BONUS_H */
