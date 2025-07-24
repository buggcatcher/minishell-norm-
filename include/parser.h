
#ifndef PARSER_H
# define PARSER_H

# include "types.h"

/* Tokenization functions */
t_token	*ft_tokenize(t_shell_state *state, t_token *token, char *input);
t_token	*ft_get_token(t_shell_state *s, t_token *t, char **i, t_token **n);
t_token	*ft_create_token(t_token_type type, char *start, int len);
t_token	*ft_word(t_shell_state *state, t_token **new, char **input);
int	process_argument_token(t_token **cur, t_token *end, char **argv, int *i);

/* Variable expansion functions */
int		ft_check_dquote(t_shell_state *state, t_token *token, char *start);
int		ft_check_var(char **input);
char	*ft_create_var(char *buffer, char **input, t_shell_state *state);
char	*ft_expand_var(char **input, t_shell_state *state);
char	*ft_my_getenv(char *var_name, t_shell_state *state);

/* Token type handlers */
t_token	*ft_pipe(t_token **new, char **input);
t_token	*ft_redher(t_token **new, char **input);
t_token	*ft_redred(t_token **new, char **input);
t_token	*ft_squote(t_token *token, t_token **new, char **input);
t_token	*ft_dquote(t_shell_state *s, t_token *t, t_token **n, char **i);

/* Token managment */
t_token	*ft_create_token_quote(t_token_type t, char *s, int l, int ws);
t_token	*ft_create_token_dollar(t_token_type t, char *b, int l, char *s);
char	*ft_process_dquote_content(t_shell_state *s, char **i, int *var);

/* Syntax checking */
int		ft_check_syntax(t_token *token);
int		ft_is_operator(t_token *token);
int	ft_check_token_space(char **input);

/* Node building */
t_node	*ft_node(t_token *token);
t_node	*ft_get_node(t_token *token);
void	ft_create_node(t_node **head, t_node *new);
void	ft_advance_tokens(t_token **start, t_token **tmp);
void	ft_add_redirection(t_node *node, t_token *token);

/* Argument building */
char	**ft_build_argv(t_token *start, t_token *end);
int		ft_count_tokens(t_token *start, t_token *end);
char	**populate_argv(char **argv, t_token *start, t_token *end);
int		ft_argv(t_token *tmp, char **argv, int i);
int	is_argument_token(t_token_type type);

/* Dummy nodes handling */
int		is_dummy_node(t_node *node);
t_node	*create_dummy_node(void);
void	append_dummy_redirs_to_node(t_node *dummy, t_node *real);
t_node	*remove_dummy_node(t_node *head, t_node *to_remove);
t_node	*prune_dummy_nodes(t_node *head);
int		has_only_redirections(t_node *node);
void	mark_node_as_dummy(t_node *node);

/* Memory cleanup */
t_node	*ft_free_nodes(t_node *head);
void	ft_free_token(t_token *token);
void	ft_free_argv(char **argv);
void	ft_free_redirs(t_redir *redir);
char	**free_argv(char **argv, int i);

#endif
