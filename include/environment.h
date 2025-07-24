#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

# include "types.h"

/* Environment initialization */
void	cleanup_node(t_env *node);
t_env	*create_env_node_m(char *env_entry);
t_env	*process_env_entry(char *env_entry, t_env *env);
t_env	*init_env_from_system(char **envp);
char	**env_to_array(t_env *env);

/* Environment manipulation */
void	set_env_var(t_env **env, char *key, char *value);
t_env	*find_env_node(t_env *env, char *key);
t_env	*create_env_node(char *key, char *value, int exported);
int		remove_env_node(t_env **env, char *key);

/* Shell state management */
void	init_shell_state(t_shell_state *state, char **envp);
void	cleanup_shell_state(t_shell_state *state);

/* Environment utilities */
char	*get_env_value(char *key, t_env *env);
int		env_size(t_env *env);
void	ft_free_env(t_env *env);
void	free_env_list(t_env *env);

#endif
