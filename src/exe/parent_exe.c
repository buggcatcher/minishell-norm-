#include "minishell.h"

int	execute_in_parent_and_close(t_node *node, t_shell_state *state, int pipe_fd[2], int prev_fd)
{
    int ret;
    
    ret= exec_in_parent(node, state);
    if (node->next)
        close(pipe_fd[1]);
    if (prev_fd != -1)
        close(prev_fd);
    return ret;
}