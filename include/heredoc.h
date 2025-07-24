
#ifndef HEREDOC_H
# define HEREDOC_H

# include "types.h"

/* Heredoc processing functions */
int		handle_heredoc_sequence(t_redir *start_redir, t_shell_state *state);
int		read_heredoc_input(char *delimiter, t_heredoc_buffer *buffer, t_shell_state *state);
int		preprocess_heredocs(t_node *nodes, t_shell_state *state);
int		append_to_buffer(t_heredoc_buffer *buffer, const char *line);
char	*expand_heredoc_line(char *line, t_shell_state *state);
void	free_heredoc_buffer(t_heredoc_buffer *buffer);
int		handle_heredoc_redirection(t_redir *redir);
int		create_fd_from_buffer(t_heredoc_buffer *buffer);
int		fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer, t_shell_state *state);
void	mark_as_skipped(t_redir *start);

/* Legacy heredoc functions */
int		ft_handle_heredoc(t_redir *redirs);
void	ft_handle_fail_heredoc_child(int pipefd[2]);
void	ft_handle_heredoc_child(int pipefd[2], char *delimiter);
int		ft_heredoc_child_process(char *delimiter, int write_fd);

/* Debug functions */
void	debug_print_nodes(t_node *node);
char	*token_type_to_string(t_token_type type);
char	*fd_to_string(int fd);

#endif
