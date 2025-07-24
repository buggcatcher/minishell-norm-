
#include "minishell.h"

volatile sig_atomic_t	g_sigint = 0;




//debug
static const char *redirect_type_to_string(int type)
{
	switch (type)
	{
		case TK_REDIR_IN_2:     return "REDIR_IN";
		case TK_REDIR_OUT_3:    return "REDIR_OUT";
		case TK_REDIR_APPEND_4: return "REDIR_APPEND";
		case TK_HEREDOC_5:      return "HEREDOC";
		default:                return "UNKNOWN";
	}
}

void debug_redirect_info(t_redir *redir)
{
	while (redir)
	{
		printf("  Redirect: %s (%d), filename=%s\n", 
			redirect_type_to_string(redir->type), redir->type, 
			redir->filename ? redir->filename : "NULL");
		redir = redir->next;
	}
}

void debug_print_nodes(t_node *node)
{
	int i = 0;
	int node_count = 0;
	t_node *current = node;
	
	if (!node)
	{
		printf("=== [DEBUG] NO NODES ===\n");
		return;
	}
	while (current)
	{
		node_count++;
		printf("---------- NODE %d ---------\n", node_count);
		if (current->argv)
		{
			i = 0;
			while (current->argv[i])
			{
				printf("argv[%d]: \"%s\"\n", i, current->argv[i]);
				i++;
			}
		}
		else
		{
			printf("argv: NULL\n");
		}
		if (current->token)
			printf("token type: %d (%s)\n", current->token->type, 
				token_type_to_string(current->token->type));
		else
			printf("token: NULL\n");
		if (current->redirs)
		{
			debug_redirect_info(current->redirs);
		}
		else
		{
			printf("Redirections: None\n");
		}
		printf("Node address: %p\n", (void *)current);
		if (current->next)
		{
			printf("Has next: Yes\n");
		}
		else
		{
			printf("Has next: No\n");
		}
		printf("-------- END NODE %d -------\n\n", node_count);
		current = current->next;
	}
	printf("[debug] total nodes: %d\n\n", node_count);
}

/// end debug

static char	*read_user_input(t_shell_state *state)
{
	char	*input;

	g_sigint = 0;
	input = readline("\033[1m\033[38;2;0;175;255m🔹minishell\033[0m\033[1m\033[38;2;95;215;0m❯\033[0m ");
	if (!input)
	{
		printf("exit\n");
		state->should_exit = true;
		return (NULL);
	}
	if (input[0] == '\0' && g_sigint)
	{
		free(input);
		return (NULL);
	}
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

static bool	parse(char *input, t_shell_state *state, t_token **token, t_node **node)
{
    *token = ft_tokenize(state, NULL, input);
    if (!*token)
        return (false);
    if (ft_check_syntax(*token) == 1)
    {
        ft_free_token(*token);
        return (false);
    }
    *node = ft_node(*token);
    if (preprocess_heredocs(*node, state) != 0)
    {
        ft_free_token(*token);
        ft_free_nodes(*node);
        return (false);
    }
    return (true);
}

static bool	process_input(char *input, t_shell_state *state)
{
    t_token	*token = NULL;
    t_node	*node = NULL;

    if (!parse(input, state, &token, &node)){
        return (false);}

	printf("\n======= DEBUG NODES =======\n");
	debug_print_nodes(node);
    ignore_signals();
    executor_loop(node, state, token);
    setup_signals();
    close_all_heredoc_fds(node);
    ft_free_nodes(node);
    ft_free_token(token);
    return (true);
}

static void	shell_loop(t_shell_state *state)
{
	char	*input;

	while (!state->should_exit)
	{
		input = read_user_input(state);
		if (!input)
			continue ;
		if (!process_input(input, state))
		{
			free(input);
			continue ;
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_state	state;

	(void)argc;
	(void)argv;
	init_shell_state(&state, envp);
	setup_signals();
	shell_loop(&state);
	cleanup_shell_state(&state);
	rl_clear_history();
	return (state.exit_code);
}
