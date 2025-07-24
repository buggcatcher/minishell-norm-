#include "minishell.h"

int	is_argument_token(t_token_type type)
{
	return (type == TK_WORD_0 || type == TK_S_QUOTE_6
		|| type == TK_D_QUOTE_7 || type == TK_DOLLAR_8);
}

static t_token	*skip_redirection_tokens(t_token *tmp, t_token *end)
{
	while (tmp && tmp != end && (tmp->type >= TK_REDIR_IN_2
		&& tmp->type <= TK_HEREDOC_5))
	{
		tmp = tmp->next;
		if (tmp == end || !tmp)
			return (NULL);
		tmp = tmp->next;
	}
	return (tmp);
}

int	ft_count_tokens(t_token *start, t_token *end)
{
	t_token	*cur;
	int		count;
	cur = start;
	count = 0;
	while (cur && cur != end)
	{
		cur = skip_redirection_tokens(cur, end);
		if (!cur || cur == end)
			break ;
		if (is_argument_token(cur->type))
		{
			count++;
			while (cur->wspace == 0 && cur->next != NULL && cur->next != end)
			{
				if (!is_argument_token(cur->type))
					break ;
				cur = cur->next;
			}
		}
		if (is_argument_token(cur->type))
			cur = cur->next;
	}
	return (count);
}

char	**populate_argv(char **argv, t_token *start, t_token *end)
{
	t_token *cur;
	int i;
	cur = start;
	i = 0;
	while (cur && cur != end)
	{
		cur = skip_redirection_tokens(cur, end);
		if (!cur || cur == end)
			break;
		if (is_argument_token(cur->type))
		{
			if (!process_argument_token(&cur, end, argv, &i))
				return (free_argv(argv, i));
			cur = skip_redirection_tokens(cur, end);
			if (!cur || cur == end)
				break;
		}
		if (cur->next)
			cur = cur->next;
		else
			break;
	}
	argv[i] = NULL;
	return (argv);
}

char	**ft_build_argv(t_token *start, t_token *end)
{
	char	**argv;
	int		j;
	j = ft_count_tokens(start, end);
	argv = safe_alloc(j + 1, sizeof(char *), "build_argv");
	if (!argv)
		return (NULL);
	argv = populate_argv(argv, start, end);
	return (argv);
}
