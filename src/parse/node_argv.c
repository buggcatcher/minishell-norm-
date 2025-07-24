#include "minishell.h"

static void		ft_concatenate_tokens(t_token *start, t_token *end, char *buffer, int first_arg)
{
	t_token	*current;
	char	*ptr;
	ptr = buffer;
	current = start;
	while (1)
	{
		ft_strcpy(ptr, current->value);
		ptr += ft_strlen(current->value);
		if (current == end)
			break ;
		current = current->next;
	}
	if (!first_arg && end->wspace == 1 && current != end)
	{
		*ptr = ' ';
		ptr++;
	}
	*ptr = '\0';
}

static char	*ft_strjoin_token_group(t_token *start, t_token *end, int first_arg)
{
	char	*result;
	size_t	total_len;
	t_token *current;
	total_len = 0;
	current = start;
    while (1)
	{
		total_len += ft_strlen(current->value);
		if (current == end)
			break;
		current = current->next;
	}
	if (!first_arg && end->wspace == 1 && current != end)
		total_len++;
	result = safe_alloc(total_len + 1, sizeof(char), "concatenate_tokens");
	if (!result)
		return (NULL);
	ft_concatenate_tokens(start, end, result, first_arg);
	return (result);
}

static t_token	*process_token_group(t_token *cur, t_token *end, char **result_str, int first_arg)
{
	t_token *group_start;
	t_token *group_end;
	group_start = cur;
	group_end = cur;
	while (cur->wspace == 0 && cur->next != NULL && cur->next != end)
	{
		cur = cur->next;
		if (!is_argument_token(cur->type))
			break;
		group_end = cur;
	}
	*result_str = ft_strjoin_token_group(group_start, group_end, first_arg);
	return (cur);
}

int	process_argument_token(t_token **cur, t_token *end, char **argv, int *i)
{
	char	*group_str;
	t_token	*processed_cur;
	int		first_arg = (*i == 0);
	group_str = NULL;
	processed_cur = process_token_group(*cur, end, &group_str, first_arg);
	if (!group_str)
		return (0);
	argv[(*i)++] = group_str;
	*cur = processed_cur;
	return (1);
}
