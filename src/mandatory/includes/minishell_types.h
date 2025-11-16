/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 01:25:08 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/16 16:30:39 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_TYPES_H
# define MINISHELL_TYPES_H

typedef struct s_data
{
	char			**envp;
	char			**argv;
	int				argc;
	int				cmd_count;
	int				isatty;
	int				last_exit_status;
}					t_data;

typedef struct s_cmd
{
	char			**argv;
	int				infd;
	int				outfd;
	int				has_error;
	int				index;
	t_data			*data;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_hdoc_ctx
{
	t_data			*data;
	int				expand;
}					t_hdoc_ctx;

#endif /* MINISHELL_TYPES_H */
