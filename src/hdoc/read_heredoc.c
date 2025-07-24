#include "minishell.h"

char	*expand_heredoc_line(char *line, t_shell_state *state)
{
	char	*input_ptr;
	char	*result;

	if (!line || !state)
		return (NULL);
	input_ptr = line;
	result = NULL;
	while (*input_ptr)
	{
		result = ft_create_var(result, &input_ptr, state);
		if (!result)
			return (NULL);
	}
	return (result);
}

static int is_heredoc_delimiter(char *line, char *delimiter, size_t delimiter_len)
{
    return (ft_strlen(line) == delimiter_len &&
            ft_strncmp(line, delimiter, delimiter_len) == 0);
}

static int expand_and_append(char *line, t_heredoc_buffer *buffer, t_shell_state *state)
{
    char *expanded_line;
	
	expanded_line = expand_heredoc_line(line, state);
    if (!expanded_line)
        return (1);
    if (append_to_buffer(buffer, expanded_line) != 0)
    {
        free(expanded_line);
        return (1);
    }
    free(expanded_line);
    return (0);
}

int read_heredoc_input(char *delimiter, t_heredoc_buffer *buffer, t_shell_state *state)
{
    char *line;
    size_t delimiter_len;
	
	delimiter_len = ft_strlen(delimiter);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (is_heredoc_delimiter(line, delimiter, delimiter_len))
        {
            free(line);
            break;
        }
        if (expand_and_append(line, buffer, state) != 0)
        {
            free(line);
            return (1);
        }
        free(line);
    }
    return (0);
}