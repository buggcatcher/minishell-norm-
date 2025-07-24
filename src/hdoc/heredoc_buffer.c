
#include "minishell.h"

static t_heredoc_buffer	*create_heredoc_buffer(void)
{
	t_heredoc_buffer	*buffer;

	buffer = safe_alloc(1, sizeof(t_heredoc_buffer), "heredoc buffer struct");
	if (!buffer)
		return (NULL);
	buffer->content = safe_alloc(4096, sizeof(char), "heredoc content buffer");
	if (!buffer->content)
	{
		free(buffer);
		return (NULL);
	}
	buffer->size = 0;
	buffer->capacity = 4096;
	buffer->content[0] = '\0';
	return (buffer);
}

static int	resize_buffer(t_heredoc_buffer *buffer, size_t new_capacity)
{
	char	*new_content;

	new_content = safe_alloc(new_capacity, sizeof(char), "heredoc resized buffer");
	if (!new_content)
		return (1);
	ft_memcpy(new_content, buffer->content, buffer->size);
	new_content[buffer->size] = '\0';
	free(buffer->content);
	buffer->content = new_content;
	buffer->capacity = new_capacity;
	return (0);
}

int	append_to_buffer(t_heredoc_buffer *buffer, const char *line)
{
	size_t line_len;
	size_t needed_size;
	size_t new_capacity ;

	line_len = strlen(line);
	needed_size = buffer->size + line_len + 1;
	if (needed_size >= buffer->capacity)
	{
		new_capacity = buffer->capacity * 2;
		while (new_capacity < needed_size)
			new_capacity *= 2;
		if (resize_buffer(buffer, new_capacity) != 0)
			return (1);
	}
	ft_strcpy(buffer->content + buffer->size, line);
	buffer->size += line_len;
	buffer->content[buffer->size] = '\n';
	buffer->size++;
	buffer->content[buffer->size] = '\0';
	return (0);
}

int	create_fd_from_buffer(t_heredoc_buffer *buffer)
{
	int	pipefd[2];
	ssize_t	bytes_written;

	if (pipe(pipefd) == -1)
		return (-1);
	bytes_written = write(pipefd[1], buffer->content, buffer->size);
	if (bytes_written == -1 || (size_t)bytes_written != buffer->size)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	close(pipefd[1]);
	return (pipefd[0]);
}

int	handle_heredoc_sequence(t_redir *start_redir, t_shell_state *state)
{
	t_heredoc_buffer	*buffer;
	int					fd;

	buffer = create_heredoc_buffer();
	if (!buffer)
		return (1);
	if (fill_heredoc_buffer(start_redir, buffer, state) != 0)
	{
		free_heredoc_buffer(buffer);
		return (1);
	}
	fd = create_fd_from_buffer(buffer);
	if (fd < 0)
	{
		free_heredoc_buffer(buffer);
		return (1);
	}
	start_redir->fd = fd;
	mark_as_skipped(start_redir->next);
	free_heredoc_buffer(buffer);
	return (0);
}


