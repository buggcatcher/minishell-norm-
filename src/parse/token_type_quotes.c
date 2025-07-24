#include "minishell.h"

t_token	*ft_dquote(t_shell_state *s, t_token *t, t_token **n, char **i)
{
	char	*start;
	char	*buffer;
	int		var;
	int		wspace;

	var = 1;
	start = *i;
	buffer = NULL;
	(*i)++;
	start = *i;
	if (ft_check_dquote(s, t, start) == 1)
		return (NULL);
	buffer = ft_process_dquote_content(s, i, &var);
	wspace = ft_check_token_space(i);
	if (var == 1)
		*n = ft_create_token_quote(TK_D_QUOTE_7, start, *i - start, wspace);
	else if (var == 2)
		*n = ft_create_token_dollar(TK_DOLLAR_8, buffer, ft_strlen_v(buffer), start);
	if (buffer)
		free(buffer);
	(*i)++;
	return (*n);
}

t_token	*ft_squote(t_token *token, t_token **new, char **input)
{
	char	*start;
	int		wspace;

	(void)token;
	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input != '\'')
		return (ft_putstr_stderr("Unclosed single quote\n"), NULL);
	wspace = ft_check_token_space(input);
	*new = ft_create_token_quote(TK_S_QUOTE_6, start, *input - start, wspace);
	(*input)++;
	return (*new);
}