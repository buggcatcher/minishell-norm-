#include "minishell.h"

t_node	*prune_dummy_nodes(t_node *head)
{
	t_node	*current;
	
	current = head;
	while (current)
	{
		if (is_dummy_node(current) && current->next
			&& !is_dummy_node(current->next))
		{
			append_dummy_redirs_to_node(current, current->next);
			head = remove_dummy_node(head, current);
			current = head;
		}
		else
		{
			current = current->next;
		}
	}
	return (head);
}

int	has_only_redirections(t_node *node)
{
	if (!node)
		return (0);
	if (node->redirs && (!node->argv || !node->argv[0] || 
		ft_strlen_v(node->argv[0]) == 0))
		return (1);
	return (0);
}

void	mark_node_as_dummy(t_node *node)
{
	if (!node)
		return ;
	if (node->argv)
		ft_free_argv(node->argv);
	node->argv = safe_alloc(2, sizeof(char *), "convert_dummy_argv");
	if (node->argv)
	{
		node->argv[0] = ft_strdup("");
		node->argv[1] = NULL;
	}
	node->dummy_on = 1;
}