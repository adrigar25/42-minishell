/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_error_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 16:27:37 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:39:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../mandatory/minishell.h"
#include <stdlib.h>

int	ft_handle_error(int error_code, int exit_code, char *msg, char *msg2)
{
	if (error_code == 1)
		ft_fprintf(2, ERROR_COMMAND_NOT_FOUND, msg);
	else if (error_code == 2)
		ft_fprintf(2, ERROR_PERMISSION_DENIED);
	else if (error_code == 3)
		ft_fprintf(2, ERROR_IS_A_DIRECTORY, msg);
	else if (error_code == 4)
		ft_fprintf(2, ERROR_NO_SUCH_FILE, msg);
	else if (error_code == 5)
		ft_fprintf(2, ERROR_SYNTAX);
	else if (error_code == 6)
		ft_fprintf(2, ERROR_SYNTAX_TOKEN, msg);
	else if (error_code == 7)
		ft_fprintf(2, ERROR_TOO_MANY_ARGS);
	else if (error_code == 8)
		ft_fprintf(2, ERROR_HOME_NOT_SET);
	else if (error_code == 9)
		ft_fprintf(2, ERROR_HEREDOC_DELIMITER);
	else if (error_code == 10)
		ft_fprintf(2, ERROR_HEREDOC_PROCESS);
	else if (error_code == 11)
		ft_fprintf(2, ERROR_CD_FAIL, msg, msg2);
	else if (error_code == 12)
		ft_fprintf(2, ERROR_AMBIGUOUS_REDIRECT, msg);
	else if (error_code == 13)
		ft_fprintf(2, ERROR_INVALID_IDENTIFIER, msg);
	else if (error_code == 14)
		ft_fprintf(2, ERROR_NUM_ARG_REQ, msg);

	return (exit_code);
}