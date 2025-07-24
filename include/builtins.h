#ifndef BUILTINS_H
# define BUILTINS_H

# include "types.h"
# include <stdbool.h>

/* Basic builtins */
int		exe_pwd(char **args, t_env **env);
int		exe_unset(char **args, t_env **env);
int		exe_env(char **arg, t_env *env);
int		exe_echo(char **args);

/* Directory navigation */
int		get_cwd_or_error(char *buf, const char *context);
char	*resolve_cd_target(char **args, t_env *env);
int		exe_cd(char **args, t_env **env);

/* Exit builtin */
int		exit_numeric_error(char *arg, t_shell_state *state);
int		exit_too_many_args(t_shell_state *state);
int		exe_exit(char **args, t_shell_state *state);

/* Export builtin */
bool	is_valid_variable_name(const char *str);
void	print_export_error(char *arg);
void	handle_export_without_value(char *arg, t_env **env);
void	handle_key_value_export(char *arg, char *equals, t_env **env);
int		handle_single_export(char *arg, t_env **env);
int		print_exported_env(t_env *env);
int		exe_export(char **args, t_env **env);

#endif
