/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:46:45 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_is_valid_identifier(const char *str)
{
	int	i;

	if (!str || *str == '\0')
		return (0);
	if (!((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')
			|| str[0] == '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A'
					&& str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')
				|| str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static void	ft_print_exported_vars(char **envp)
{
	int		i;
	int		j;
	char	*name;
	char	*value;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && envp[i][j] != '=')
			j++;
		name = ft_substr(envp[i], 0, j);
		if (envp[i][j] == '=')
		{
			value = ft_substr(envp[i], j + 1, ft_strlen(envp[i]) - j - 1);
			ft_fprintf(1, "declare -x %s=\"%s\"\n", name, value);
			free(value);
		}
		else
			ft_fprintf(1, "declare -x %s\n", name);
		free(name);
		i++;
	}
}

static int	ft_export_variable(const char *arg, char ***envp)
{
	char	*name;
	char	*value;
	char	*equals_pos;
	int		result;

	if (!ft_is_valid_identifier(arg))
		return (ft_handle_error(13, 1, NULL, NULL));
	equals_pos = ft_strchr(arg, '=');
	if (equals_pos)
	{
		name = ft_substr((char *)arg, 0, equals_pos - arg);
		value = ft_strdup(equals_pos + 1);
		result = ft_setenv(name, value, envp);
		free(name);
		free(value);
		if (result == 0)
			return (0);
		else
			return (1);
	}
	else
	{
		if (!ft_getenv(arg, *envp))
		{
			result = ft_setenv((char *)arg, "", envp);
			if (result == 0)
				return (0);
			else
				return (1);
		}
		result = ft_setenv((char *)arg, "", envp);
		if (result == 0)
			return (0);
		else
			return (1);
	}
	return (0);
}

int	ft_export(char **argv, char ***envp)
{
	int	i;
	int	exit_status;

	if (!argv[1])
	{
		ft_print_exported_vars(*envp);
		return (0);
	}
	exit_status = 0;
	i = 1;
	while (argv[i])
	{
		if (ft_export_variable(argv[i], envp) != 0)
			exit_status = 1;
		i++;
	}
	return (exit_status);
}
