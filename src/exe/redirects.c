#include "minishell.h"

static int	open_redirection_file(t_redir *redir)
{
	int	fd;
	if (redir->type == TK_REDIR_IN_2)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == TK_REDIR_OUT_3)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TK_REDIR_APPEND_4)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	return (fd);
}

static int	get_redirection_target(int type)
{
	if (type == TK_REDIR_IN_2 || type == TK_HEREDOC_5)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

static int	process_redirection(t_redir *redir)
{
	int	fd;
	int	target;
	if (redir->type == TK_HEREDOC_5)
	{
		if (redir->fd == -2)
			return (0);
		fd = redir->fd;
	}
	else
	{
		fd = open_redirection_file(redir);
		if (fd < 0)
			return (printf("minishell: %s: No such file or directory\n", redir->filename), 1);
	}
	target = get_redirection_target(redir->type);
	if (switch_fd(fd, target) != 0)
	{
		close(fd);
		redir->fd = -1;
		return (1);
	}
	close(fd);
	redir->fd = -1;
	return (0);
}

int	apply_redirects(t_redir *redirs)
{
	while (redirs)
	{
		if (process_redirection(redirs) != 0)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}

void	update_fds_for_next_cmd(t_node *node, int pipe_fd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (node->next)
	{
		*prev_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
}
