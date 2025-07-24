#include "minishell.h"

void	ft_free_env(t_env *env)
{
	t_env	*tmp;
	while (env)
	{
		tmp = env;
		env = env->next;
		if (tmp->key)
			free(tmp->key);
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

t_node	*ft_free_nodes(t_node *head)
{
	t_node	*tmp;
	while (head)
	{
		tmp = head;
		head = head->next;
		ft_free_argv(tmp->argv);
		ft_free_redirs(tmp->redirs);
		free(tmp);
	}
	return (NULL);
}

void	ft_free_token(t_token *token)
{
	t_token	*tmp;
	while (token)
	{
		tmp = token;
		token = token->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

void	ft_free_argv(char **argv)
{
	int	i;
	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void ft_free_redirs(t_redir *redir)
{
    t_redir *next_redir;
    while (redir)
    {
        next_redir = redir->next;
        free(redir->filename);
        if (redir->heredoc_buffer)
            free_heredoc_buffer(redir->heredoc_buffer);
        free(redir);
        redir = next_redir;
    }
}

