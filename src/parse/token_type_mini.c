#include "minishell.h"

t_token	*ft_create_token_quote(t_token_type t, char *s, int l, int ws)
{
	t_token	*new;
	new = safe_alloc(1, sizeof(t_token), "create_token_quote");
	if (!new)
		return (NULL);
	new->type = t;
	new->value = ft_strndup(s, l);
	new->wspace = ws;
	new->next = NULL;
	return (new);
}

t_token	*ft_create_token_dollar(t_token_type t, char *b, int l, char *s)
{
	t_token	*new;
	char	*wspaceptr;
	new = safe_alloc(1, sizeof(t_token), "create_token_word");
	if (!new)
		return (NULL);
	new->type = t;
	new->value = ft_strndup(b, l);
	new->wspace = 0;
	wspaceptr = s;
	while (*wspaceptr && \
			((*wspaceptr >= '0' && *wspaceptr <= '9') || \
			(*wspaceptr >= 'A' && *wspaceptr <= 'Z') || \
			(*wspaceptr >= 'a' && *wspaceptr <= 'z') || \
			*wspaceptr == '_' || *wspaceptr == '$'))
		wspaceptr++;
	if (*wspaceptr == '"' )
		wspaceptr++;
	if (*wspaceptr == ' ')
		new->wspace = 1;
	new->next = NULL;
	return (new);
}

char	*ft_process_dquote_content(t_shell_state *s, char **i, int *var)
{
	char	*buffer;
	buffer = NULL;
	while (**i && **i != '"')
	{
		if (*var != 2)
			*var = ft_check_var(i);
		buffer = ft_create_var(buffer, i, s);
	}
	return (buffer);
}

t_token	*ft_create_token(t_token_type type, char *start, int len)
{
	t_token	*new;
	
	new = safe_alloc(1, sizeof(t_token), "create_token_basic");
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->next = NULL;
	new->wspace = 0;
	return (new);
}