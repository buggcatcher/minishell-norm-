#include "minishell.h"

static void	handle_redirections(t_node *node, t_env *env, t_node *head, t_token *token_head)
{
	if (node->redirs && apply_redirects(node->redirs) != 0)	
	{
		ft_free_token(token_head);
		ft_free_nodes(head);
		ft_free_env(env);
		exit(1);
	}
}

static void	handle_builtin(t_node *node, t_env **env, t_token *token_head)
{
	int	status;
	
	if (is_builtin(node->argv[0]))
	{
		status = exec_builtin(node->argv, env);
		clean_exit(node, *env, status, token_head);
	}
}

static char	*get_bin_path(t_node *node, t_env *env, t_node *head, t_token *token_head)
{
    char *bin;

    if (ft_strchr(node->argv[0], '/'))
        bin = node->argv[0];
    else if (node->argv[0][0] == '.' && node->argv[0][1] == '\0')
    {
        write(2, "command not found\n", 19);
        ft_free_token(token_head);
        ft_free_nodes(head);
        ft_free_env(env);
        exit(127);
    }
    else
        bin = resolve_path(node->argv[0], env, node, head);
    return bin;
}

static void	execute_command(t_node *node, t_env *env, t_node *head, t_token *token_head)
{
    char	*bin;
    char	**env_arr;

    if (!node->argv || !node->argv[0])
    {
        ft_free_token(token_head);
        ft_free_nodes(node);
        exit(127);
    }
    bin = get_bin_path(node, env, head, token_head);
    if (!bin)
    {
        ft_free_token(token_head);
        ft_free_nodes(node);
        ft_free_env(env);
        exit(127);
    }
    env_arr = env_to_array(env);
    execve(bin, node->argv, env_arr);
    write(2, "command not found\n", 19);
    ft_free_token(token_head);
    free_array(env_arr);
    ft_free_nodes(node);
    ft_free_env(env);
    exit(127);
}

void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_child_context *ctx)
{
	disable_signals();
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);
	handle_pipes(pipe_in, pipe_out);
	handle_redirections(node, ctx->env, ctx->head, ctx->token_head);
	if (!node->argv || !node->argv[0])
	{
		ft_free_token(ctx->token_head);
		ft_free_nodes(node);
		ft_free_env(ctx->env);
		exit(0);
	}
	handle_builtin(node, &ctx->env, ctx->token_head);
	execute_command(node, ctx->env, ctx->head, ctx->token_head);
}
