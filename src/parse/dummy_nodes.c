#include "minishell.h"

int	is_dummy_node(t_node *node)
{
	if (!node || !node->argv || !node->argv[0])
		return (1);
	if (ft_strlen_v(node->argv[0]) == 0)
		return (1);
	if (node->dummy_on == 1)
		return (1);
	return (0);
}

t_node	*create_dummy_node(void)
{
	t_node	*dummy;
	
	dummy = safe_alloc(1, sizeof(t_node), "create_dummy_node");
	if (!dummy)
		return (NULL);
	dummy->redirs = NULL;
	dummy->token = NULL;
	dummy->argv = safe_alloc(2, sizeof(char *), "dummy_argv");
	if (!dummy->argv)
	{
		free(dummy);
		return (NULL);
	}
	dummy->argv[0] = ft_strdup("");
	dummy->argv[1] = NULL;
	dummy->dummy_on = 1;
	dummy->next = NULL;
	return (dummy);
}

void	append_dummy_redirs_to_node(t_node *dummy, t_node *real)
{
	t_redir	*dummy_redir;
	t_redir	*last_redir;
	
	if (!dummy || !real || !is_dummy_node(dummy))
		return ;
	if (dummy->redirs)
	{
		dummy_redir = dummy->redirs;
		if (!real->redirs)
			real->redirs = dummy_redir;
		else
		{
			last_redir = real->redirs;
			while (last_redir->next)
				last_redir = last_redir->next;
			last_redir->next = dummy_redir;
		}
		while (dummy_redir)
		{
			dummy_redir->node = real;
			dummy_redir = dummy_redir->next;
		}
		dummy->redirs = NULL;
	}
}

static t_node	*unlink_node(t_node *head, t_node *to_remove)
{
    t_node	*current = head;
    t_node	*prev = NULL;

    if (!head || !to_remove)
        return (head);
    if (head == to_remove)
        return (head->next);
    while (current && current != to_remove)
    {
        prev = current;
        current = current->next;
    }
    if (current == to_remove && prev)
        prev->next = current->next;
    return (head);
}

t_node	*remove_dummy_node(t_node *head, t_node *to_remove)
{
    if (!head || !to_remove)
        return (head);
    head = unlink_node(head, to_remove);
    to_remove->next = NULL;
    ft_free_nodes(to_remove);
    return (head);
}

