#include "minishell.h"

static int	handle_redirs_and_builtin(t_node *node, t_shell_state *state)
{
    if (!node->argv || !node->argv[0])
    {
        if (node->redirs && apply_redirects(node->redirs))
            state->last_status = 1;
        else
            state->last_status = 0;
    }
    else
    {
        if (node->redirs && apply_redirects(node->redirs))
            state->last_status = 1;
        else
            state->last_status = exec_builtin(node->argv, &state->env);
    }
    return (state->last_status);
}

int exec_in_parent(t_node *node, t_shell_state *state)
{
    int save_out;
    int save_in;

    save_out = save_fd(STDOUT_FILENO);
    save_in = save_fd(STDIN_FILENO);
    if (!node->argv || !node->argv[0])
        handle_redirs_and_builtin(node, state);
    else if (!ft_strcmp(node->argv[0], "exit"))
    {
        close(save_out);
        close(save_in);
        exe_exit(node->argv, state);
        return (state->last_status);
    }
    else
        handle_redirs_and_builtin(node, state);
    switch_fd(save_out, STDOUT_FILENO);
    switch_fd(save_in, STDIN_FILENO);
    close(save_out);
    close(save_in);
    return (state->last_status);
}

int	wait_for_last(pid_t last_pid, int *exit_status)
{
	int		status;
	pid_t	pid;
	int		signal;
	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == 0)
			continue ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				signal = WTERMSIG(status);
				*exit_status = 128 + signal;
				if (signal == SIGINT || signal == SIGQUIT)
					ft_putstr_stderr("\n");
			}
		}
	}
	return (*exit_status);
}

static int	handle_fork_and_child(t_executor_context *ctx)
{
    pid_t	pid;
    t_child_context	child_ctx;

    pid = fork();
    if (pid == -1)
        return (write(2, "error: Fork\n", 6), 1);
    if (pid == 0)
    {
        child_ctx.env = ctx->state->env;
        child_ctx.head = ctx->head;
        child_ctx.token_head = ctx->token_head;
        exec_child(ctx->node, ctx->pipe_fd, ctx->prev_fd, &child_ctx);
    }
    else
        *(ctx->last_pid) = pid;
    return (0);
}

int	executor_loop(t_node *node, t_shell_state *state, t_token *token_head)
{
    pid_t	last_pid;
    t_node  *head;
    t_executor_context ctx;
    int		prev_fd;
    int		pipe_fd[2];
    

    last_pid = -1;
    prev_fd = -1;
    head = node;
    while (node)
    {
        if (create_pipe_if_needed(node, pipe_fd))
            return (1);
        if (should_execute_in_parent(node))
            return (exec_in_parent(node, state));
        ctx = (t_executor_context){node, head, state, token_head, pipe_fd, prev_fd, &last_pid};
        if (handle_fork_and_child(&ctx))
            return (1);
        update_fds_for_next_cmd(node, pipe_fd, &prev_fd);
        node = node->next;
    }
    return (wait_for_last(last_pid, &state->last_status));
}
