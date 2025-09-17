#include "../../minishell_bonus.h"
#include <signal.h>
#include <unistd.h>

void	ft_setup_child_io(t_cmd *current, t_cmd *cmd_list)
{
	t_cmd	*temp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (current->infd != STDIN_FILENO)
		dup2(current->infd, STDIN_FILENO);
	if (current->outfd != STDOUT_FILENO)
		dup2(current->outfd, STDOUT_FILENO);
	temp = cmd_list;
	while (temp)
	{
		if (temp != current)
		{
			if (temp->infd != STDIN_FILENO && temp->infd != current->infd
				&& temp->infd != current->outfd)
				close(temp->infd);
			if (temp->outfd != STDOUT_FILENO && temp->outfd != current->infd
				&& temp->outfd != current->outfd)
				close(temp->outfd);
		}
		else
		{
			if (temp->infd != STDIN_FILENO && temp->infd != current->infd)
				close(temp->infd);
			if (temp->outfd != STDOUT_FILENO && temp->outfd != current->outfd)
				close(temp->outfd);
		}
		temp = temp->next;
	}
}
