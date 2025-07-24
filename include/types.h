
#ifndef TYPES_H
# define TYPES_H

# include <stddef.h>

/* Token types */
typedef enum e_token_type
{
	TK_WORD_0,
	TK_PIPE_1,
	TK_REDIR_IN_2,
	TK_REDIR_OUT_3,
	TK_REDIR_APPEND_4,
	TK_HEREDOC_5,
	TK_S_QUOTE_6,
	TK_D_QUOTE_7,
	TK_DOLLAR_8,
}	t_token_type;

/* Forward declarations */
typedef struct s_node t_node;
typedef struct s_heredoc_buffer t_heredoc_buffer;

/* Token structure */
typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				wspace;
	struct s_token	*next;
}	t_token;

/* Redirection structure */
typedef struct s_redir
{
	int						type;
	char					*filename;
	int						fd;
	t_node					*node;
	t_heredoc_buffer		*heredoc_buffer;
	struct s_redir			*next;
	struct s_redir			*last;
}	t_redir;

/* AST Node structure */
typedef struct s_node
{
	t_redir			*redirs;
	t_token			*token;
	char			**argv;
	int				dummy_on;
	struct s_node	*next;
}	t_node;

/* Environment variable structure */
typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

/* Shell state structure */
typedef struct s_shell_state
{
	t_env	*env;
	int		last_status;
	int		should_exit;
	int		exit_code;
}	t_shell_state;

/* Heredoc buffer structure */
typedef struct s_heredoc_buffer
{
	char	*content;
	size_t	size;
	size_t	capacity;
}	t_heredoc_buffer;

/* Child execution context structure */
typedef struct s_child_context
{
	t_env			*env;
	t_node			*head;
	t_token			*token_head;
}	t_child_context;

typedef struct s_executor_context
{
    t_node			*node;
    t_node			*head;
    t_shell_state	*state;
    t_token			*token_head;
    int				*pipe_fd;
    int				prev_fd;
    pid_t			*last_pid;
}	t_executor_context;

#endif
