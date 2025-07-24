#include "minishell.h"

char	**free_argv(char **argv, int i)
{
	while (i-- > 0)
		free(argv[i]);
	free(argv);
	return (NULL);
}

void	free_array(char **arr)
{
	int	i;
	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_heredoc_buffer(t_heredoc_buffer *buffer)
{
	if (!buffer)
		return;
	if (buffer->content)
		free(buffer->content);
	free(buffer);
}

void	close_all_heredoc_fds(t_node *head)
{
    t_node *cur;
	t_redir *redir;

	cur = head;
    while (cur)
    {
        redir = cur->redirs;
        while (redir)
        {
            if (redir->type == TK_HEREDOC_5 && redir->fd > 2)
            {
                close(redir->fd);
                redir->fd = -1;
            }
            redir = redir->next;
        }
        cur = cur->next;
    }
}

void	clean_exit(t_node *node, t_env *env, int status, t_token *token_head)
{
	ft_free_token(token_head);
	ft_free_nodes(node);
	ft_free_env(env);
	exit(status);
}