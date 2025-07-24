#include "minishell.h"

char	*join_key_value(char *key, char *value)
{
	size_t	key_len;
	size_t	val_len;
	char	*str;
	if (!key || !value)
		return (NULL);
	key_len = strlen(key);
	val_len = strlen(value);
	str = safe_alloc(key_len + val_len + 2, sizeof(char), "join_key_value");
	if (!str)
		return (NULL);
	ft_memcpy(str, key, key_len);
	str[key_len] = '=';
	ft_memcpy(str + key_len + 1, value, val_len);
	str[key_len + 1 + val_len] = '\0';
	return (str);
}

int	exec_builtin(char **args, t_env **env)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		return (exe_cd(args, env));
	else if (!ft_strcmp(args[0], "export"))
		return (exe_export(args, env));
	else if (!ft_strcmp(args[0], "unset"))
		return (exe_unset(args, env));
	else if (!ft_strcmp(args[0], "pwd"))
		return (exe_pwd(args, env));
	else if (!ft_strcmp(args[0], "env"))
		return (exe_env(args, *env));
	else if (!ft_strcmp(args[0], "echo"))
		return (exe_echo(args));
	return (1);
}

int	create_pipe_if_needed(t_node *node, int pipe_fd[2])
{
	if (node->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			write(2, "Pipe\n", 6);
			return (1);
		}
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}
