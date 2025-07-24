#include "minishell.h"

static bool	is_invalid_cmd(char *cmd)
{
	if (cmd == NULL || cmd[0] == '\0')
	{
		write(2, "command not found\n", 19);
		return (true);
	}
	return (false);
}

static bool	is_absolute_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

static int	has_heredoc_before_redirect_out(t_redir *redir_list)
{
	t_redir	*current;
	current = redir_list;
	while (current)
	{
		if (current->type == TK_HEREDOC_5 && current->next != NULL)
			return (1);
			
		current = current->next;
	}
	return (0);
}

static void ft_free_cmd_not_found(t_env *env, t_node *node)
{
	if (node && node->token)
		ft_free_token(node->token);
	ft_free_nodes(node);
	ft_free_env(env);
}

char	*resolve_path(char *cmd, t_env *env, t_node *node, t_node *head)
{
	char	**paths;
	char	*full_path;
	if (is_invalid_cmd(cmd))
		return (NULL);
	if (is_absolute_path(cmd))
		return (ft_strdup(cmd));
	paths = get_paths(env);
	if (!paths)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	free_split_all(paths);
	if (!full_path && !has_heredoc_before_redirect_out(node->redirs))
	{
		close_all_heredoc_fds(head);
		ft_free_cmd_not_found(env, head);
		write(2, "command not found\n", 19);
		exit(127); 
	}
	return (full_path);
}
 