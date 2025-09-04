/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msg_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:58:40 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:18:43 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_dot_or_dotdot(const char *name)
{
	return (ft_strcmp(name, ".") == 0 || ft_strcmp(name, "..") == 0);
}

static char	*ft_build_path(const char *dir, const char *entry)
{
	char path[4096];
	snprintf(path, sizeof(path), "%s/%s", dir, entry);
	return ft_strdup(path);
}

static char	*ft_search_in_subdirs(const char *dir, const char *filename)
{
	DIR *d = opendir(dir);
	if (!d) return NULL;
	struct dirent *entry;
	char *result = NULL;

	while ((entry = readdir(d)))
	{
		if (ft_is_dot_or_dotdot(entry->d_name))
			continue;
		char *sub_path = ft_build_path(dir, entry->d_name);
		DIR *subdir = opendir(sub_path);
		if (subdir)
		{
			closedir(subdir);
			result = ft_search_file(sub_path, filename);
			free(sub_path);
			if (result)
				break;
		}
		else
			free(sub_path);
	}
	closedir(d);
	return result;
}

static char	*ft_search_in_dir(const char *dir, const char *filename)
{
	DIR *d = opendir(dir);
	if (!d) return NULL;
	struct dirent *entry;
	char *result = NULL;

	while ((entry = readdir(d)))
	{
		if (ft_is_dot_or_dotdot(entry->d_name))
			continue;
		if (ft_strcmp(entry->d_name, filename) == 0)
		{
			result = ft_build_path(dir, entry->d_name);
			break;
		}
	}
	closedir(d);
	if (!result)
		result = ft_search_in_subdirs(dir, filename);
	return result;
}

char *ft_search_file(const char *dir, const char *filename)
{
	const char *start_dir = dir ? dir : getenv("PWD");
	return ft_search_in_dir(start_dir, filename);
}

int	ft_msg_start(void)
{
	char *welcome_msg;

	welcome_msg = ft_search_file(NULL, "welcome.txt");
	system("clear");
	if (welcome_msg)
	{
		ft_print_file(welcome_msg, "\033[0;32m");
		free(welcome_msg);
	}
	else
		ft_print_file(WELCOME_MSG_TXT, "\033[0;32m");
	ft_putstr("\n");
	ft_putstr("\033[0;34mWelcome to Minishell!\033[0m\n");
	ft_putstr("\n\n");
	return (0);
}
