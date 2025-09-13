/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 15:09:46 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_pwd(t_cmd cmd)
{
	char	cwd[1024];
	char	*output;
	int		len;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		len = ft_strlen(cwd);
		output = malloc(len + 2);
		if (!output)
			return (1);
		ft_strcpy(output, cwd);
		output[len] = '\n';
		output[len + 1] = '\0';

		write(cmd.outfd, output, len + 1);

		free(output);
		return (0);
	}
	else
	{
		perror("minishell: pwd");
		return (1);
	}
}
