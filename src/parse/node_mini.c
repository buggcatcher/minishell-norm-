#include "minishell.h"

void	ft_create_node(t_node **head, t_node *new)
{
	t_node	*tail;
	if (!*head)
	{
		*head = new;
	}
	else
	{
		tail = *head;
		while (tail->next)
			tail = tail->next;
		tail->next = new;
	}
}

void	ft_advance_tokens(t_token **start, t_token **tmp)
{
	if (*tmp && (*tmp)->type == TK_PIPE_1)
	{
		*start = (*tmp)->next;
		*tmp = (*tmp)->next;
	}
}

static void	init_redir(t_redir *redir, int type, t_node *node)
{
	redir->node = node;
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
	redir->last = NULL;
	redir->heredoc_buffer = NULL;
}

static char	*ft_strdup_trim_last_space(char *s)
{
	size_t	len;
	char	*dup;
	if (!s)
		return (NULL);
	len = ft_strlen_v(s);
	if (len == 0)
		return (ft_strdup(""));
	if (s[len - 1] == ' ')
	{
		dup = (char *)safe_alloc(len, sizeof(char), "ft_strtrim_space");
		if (!dup)
			return (NULL);
		ft_memcpy(dup, s, len - 1);
		dup[len - 1] = '\0';
		return (dup);
	}
	return (ft_strdup(s));
}

void	ft_add_redirection(t_node *node, t_token *token)
{
	t_redir	*new_redir;
	t_redir	*last;
	new_redir = safe_alloc(1, sizeof(t_redir), "add_redirection");
	if (!new_redir)
		return ;
	init_redir(new_redir, token->type, node);
	if (token->next && (token->next->type == TK_WORD_0 || \
			token->next->type == TK_S_QUOTE_6 || \
			token->next->type == TK_D_QUOTE_7 || \
			token->next->type == TK_DOLLAR_8))
		new_redir->filename = ft_strdup_trim_last_space(token->next->value);
	else
		new_redir->filename = NULL;
	if (!node->redirs)
		node->redirs = new_redir;
	else
	{
		last = node->redirs;
		while (last->next)
			last = last->next;
		last->next = new_redir;
		new_redir->last = last;
	}
}
