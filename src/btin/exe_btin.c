#include "minishell.h"

int	exe_pwd(char **args, t_env **env)
{
	char	cwd[PATH_MAX + 1];
	(void)args;
	(void)env;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	write(2, "pwd\n", 5);
	return (1);
}

int	exe_unset(char **args, t_env **env)
{
	int	i;
	int	exit_status;
	i = 1;
	exit_status = 0;
	if (!args[1])
		return (write(2, "unset: not enough arguments\n", 28), 1);
	while (args[i])
	{
		if (remove_env_node(env, args[i]) != 0)
			write(2, "unset: remove node failed\n", 27);
		i++;
	}
	return (exit_status);
}

int	exe_env(char **arg, t_env *env)
{
	
	if (arg[1] != NULL)
		return (printf("env: ‘%s’: no option or argument required\n", arg[1]), 0);
	while (env)
	{
		if (env->exported && env->value != NULL)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

static int	ft_echo_flag(char **arg)
{
	int	i;
	int	j;
	i = 1;
	while (arg[i])
	{
		j = 1;
		if (arg[i][0] != '-' || arg[i][1] == '\0')
			return (i);
		while (arg[i][j] == 'n')
			j++;
		while (arg[i][j] == ' ')
			j++;
		if (arg[i][j] != '\0')
			return (i);
		i++;
	}
	return (i);
}

int	exe_echo(char **args)
{
	int i;
	int new_line;
  
	new_line = ft_echo_flag(args);
	if (new_line > 1)
		i = new_line;
	else
		i = 1;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1] != NULL)
			printf(" ");
		i++;
	}
	if (new_line <= 1)
		printf("\n");
	return (0);
}
