/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_generate_prompt.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/07 11:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 10:25:25 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_generate_prompt(char **envp)
{
	char	*dir_path;
	char	*temp;
	char	*prompt;

	dir_path = ft_get_directory_path(envp);
	if (!dir_path)
		return (ft_strdup("minishell> "));
	temp = ft_strjoin(PROMPT_PREFIX, dir_path);
	free(dir_path);
	if (!temp)
		return (ft_strdup("minishell> "));
	prompt = ft_strjoin(temp, PROMPT_SUFFIX);
	free(temp);
	if (!prompt)
		return (ft_strdup("minishell> "));
	return (prompt);
}
