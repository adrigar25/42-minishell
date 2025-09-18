#include "../../minishell.h"
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
	ft_close_unused_fds(current, cmd_list);
}
