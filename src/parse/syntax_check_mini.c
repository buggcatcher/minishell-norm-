#include "minishell.h"

int	ft_check_syntax(t_token *token)
{
	t_token	*tmp;
	if (!token)
		return (1);
	if (ft_is_operator(token))
		return (write(2, "minishell: syntax error\n", 25), 1);
	tmp = token;
	while (tmp && tmp->next)
	{
		if (ft_is_operator(tmp) && ft_is_operator(tmp->next))
			return (write(2, "minishell: syntax error\n", 25), 1);
		tmp = tmp->next;
	}
	if (ft_is_operator(tmp))
		return (write(2, "minishell: syntax error\n", 25), 1);
	return (0);
}

int	ft_is_operator(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TK_PIPE_1)
		return (1);
	if (token->type == TK_REDIR_IN_2 && token->next == NULL)
		return (1);
	if (token->type == TK_REDIR_OUT_3 && token->next == NULL)
		return (1);
	if (token->type == TK_REDIR_APPEND_4 && token->next == NULL)
		return (1);
	if (token->type == TK_HEREDOC_5 && token->next == NULL)
		return (1);
	return (0);
}

int	ft_check_token_space(char **input)
{
	int		wspace;
	char	*wspaceptr;
	wspace = 0;
	wspaceptr = *input;
	wspaceptr++;
	if (*wspaceptr == ' ')
		wspace = 1;
	return (wspace);
}
