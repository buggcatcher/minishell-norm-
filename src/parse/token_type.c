#include "minishell.h"

static t_token	*ft_create_token_word(t_token_type type, char *start, int len)
{
	t_token	*new;
	char	*wspaceptr;

	new = safe_alloc(1, sizeof(t_token), "create_token_operator");
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->wspace = 0;
	wspaceptr = start;
	while (len > 0)
	{
		wspaceptr++;
		len--;
	}
	if (*wspaceptr == ' ')
		new->wspace = 1;
	new->next = NULL;
	return (new);
}

t_token	*ft_word(t_shell_state *state, t_token **new, char **input)
{
	char	*start;
	char	*buffer;
	int		var;
	t_token	*result;

	start = *input;
	buffer = NULL;
	var = 1;
	result = NULL;
	while (**input && **input != ' ' && **input != '|' && **input != '<' && \
			**input != '>' && **input != '\'' && **input != '"')
	{
		if (var != 2)
			var = ft_check_var(input);
		buffer = ft_create_var(buffer, input, state);
	}
	if (var == 1)
		result = ft_create_token_word(TK_WORD_0, start, *input - start);
	else if (var == 2)
		result = ft_create_token_dollar(TK_DOLLAR_8, buffer, ft_strlen_v(buffer), start);
	free(buffer);
	*new = result;
	return (result);
}

t_token	*ft_pipe(t_token **new, char **input)
{
	*new = ft_create_token(TK_PIPE_1, *input, 1);
	(*input)++;
	return (*new);
}

t_token	*ft_redher(t_token **new, char **input)
{
	if (*(*input + 1) == '<')
	{
		*new = ft_create_token(TK_HEREDOC_5, *input, 2);
		*input += 2;
	}
	else
	{
		*new = ft_create_token(TK_REDIR_IN_2, *input, 1);
		(*input)++;
	}
	return (*new);
}

t_token	*ft_redred(t_token **new, char **input)
{
	if (*(*input + 1) == '>')
	{
		*new = ft_create_token(TK_REDIR_APPEND_4, *input, 2);
		*input += 2;
	}
	else
	{
		*new = ft_create_token(TK_REDIR_OUT_3, *input, 1);
		(*input)++;
	}
	return (*new);
}
