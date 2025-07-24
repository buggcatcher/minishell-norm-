#include "minishell.h"

void	handle_export_without_value(char *arg, t_env **env)
{
	t_env	*existing;
	t_env	*new_node;
	existing = find_env_node(*env, arg);
	if (existing)
		existing->exported = 1;
	else
	{
		new_node = create_env_node(arg, NULL, 1);
		if (new_node)
		{
			new_node->next = *env;
			*env = new_node;
		}
	}
}

void	handle_key_value_export(char *arg, char *equals, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;
	t_env	*new_node;
	*equals = '\0';
	key = arg;
	value = equals + 1;
	existing = find_env_node(*env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = ft_strdup(value);
		existing->exported = 1;
	}
	else
	{
		new_node = create_env_node(key, value, 1);
		if (new_node)
		{
			new_node->next = *env;
			*env = new_node;
		}
	}
	*equals = '=';
}

static int	process_export_arg(char *arg, t_env **env, int has_value, char *equals)
{
    if (!is_valid_variable_name(arg))
    {
        print_export_error(arg);
        return (1);
    }
    if (has_value)
        handle_key_value_export(arg, equals, env);
    else
        handle_export_without_value(arg, env);
    return (0);
}

int	handle_single_export(char *arg, t_env **env)
{
    char	*equals;
    int		export_failed;
    char	temp;

    export_failed = 0;
    equals = strchr(arg, '=');
    if (equals)
    {
        temp = *equals;
        *equals = '\0';
        export_failed = process_export_arg(arg, env, 1, equals);
        *equals = temp;
    }
    else
    {
        export_failed = process_export_arg(arg, env, 0, NULL);
    }
    return (export_failed);
}

int	exe_export(char **args, t_env **env)
{
	int	i;
	int	export_failed;
	int	status;
	if (!args[1])
		return (print_exported_env(*env));
	i = 1;
	export_failed = 0;
	while (args[i])
	{
		status = handle_single_export(args[i], env);
		if (status != 0)
			export_failed = 1;
		i++;
	}
	return (export_failed);
}
