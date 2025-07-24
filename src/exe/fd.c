#include "minishell.h"

void	handle_pipes(int pipe_in, int pipe_out[2])
{
	if (pipe_in != -1)
		switch_fd(pipe_in, STDIN_FILENO);
	if (pipe_out[0] != -1)
		close(pipe_out[0]);
	if (pipe_out[1] != -1)
		close(pipe_out[1]);
	if (pipe_in != -1)
		close(pipe_in);
}

int	switch_fd(int src, int dest)
{
	if (src != dest)
	{
		if (dup2(src, dest) == -1)
		{
			write(2, "Dup2\n", 6);
			return (1);
		}
	}
	return (0);
}

int	save_fd(int fd)
{
	int	clone;
	clone = dup(fd);
	if (clone == -1)
	{
		write(2, "Dup\n", 5);
		exit(EXIT_FAILURE);
	}
	return (clone);
}

int	save_stdout(void)
{
	int	clone;
	clone = dup(1);
	if (clone == -1)
	{
		write(2, "Dup stdout\n", 12);
		exit(EXIT_FAILURE);
	}
	return (clone);
}

int	open_outfile( char *filename)
{
	int	fd;
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, "Error opening outfile\n", 23);
		return (-1);
	}
	return (fd);
}
