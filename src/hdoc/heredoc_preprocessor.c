
#include "minishell.h"

int preprocess_heredocs(t_node *node, t_shell_state *state)
{
    t_node *current;
    t_redir *redir;

    current = node;
    while (current)
    {
        redir = current->redirs;
        while (redir)
        {
            if (redir->type == TK_HEREDOC_5 && redir->fd == -1)
            {
                if (handle_heredoc_sequence(redir, state) != 0)
                    return (1);
            }
            redir = redir->next;
        }       
        current = current->next;
    }
    return (0);
}

int fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer, t_shell_state *state)
{
    t_redir *current;
    
    current = start;
    while (current && current->type == TK_HEREDOC_5)
    {
        buffer->size = 0;
        if (buffer->capacity > 0 && buffer->content)
        {
            buffer->content[0] = '\0';
        }
        if (read_heredoc_input(current->filename, buffer, state) != 0)
            return (1);
        current = current->next;
    }
    return (0);
}

void mark_as_skipped(t_redir *start)
{
    t_redir *current;
    
    current = start;
    while (current && current->type == TK_HEREDOC_5)
    {
        current->fd = -2;
        current = current->next;
    }
}







