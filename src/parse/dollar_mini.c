#include "minishell.h"

static char	*ft_expand_normal_var(char **input, t_shell_state *state, char *start)
{
	char	*var_name;
	char	*var_value;
	while (**input && ((**input >= '0' && **input <= '9') || \
					(**input >= 'A' && **input <= 'Z') || \
					(**input >= 'a' && **input <= 'z') || \
					**input == '_'))
			(*input)++;
	if (*input == start)
		return (ft_strdup("$"));
	var_name = ft_strndup(start, *input - start);
	var_value = ft_my_getenv(var_name, state);
	free(var_name);
	if (!var_value)
		var_value = "";
	return (ft_strdup(var_value));
}

static int	get_pid_from_proc_self(void)
{
	char buffer[256];
	int fd;
	ssize_t bytes;
	int pid;
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
    	return -1;
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes <= 0)
    	return -1;
	buffer[bytes] = '\0';
	pid = 0;
	int i = 0;
	while (buffer[i] >= '0' && buffer[i] <= '9')
	{
    	pid = pid * 10 + (buffer[i] - '0');
    	i++;
	}
	return (pid);
}

char	*ft_expand_var(char **input, t_shell_state *state)
{
	char	*start;
	(*input)++;
	start = *input;
	if (**input == '?')
	{
		(*input)++;
		return (ft_itoa(state->last_status));
	}
	if (**input == '$')
	{
		(*input)++;
		return (ft_itoa(get_pid_from_proc_self()));
	}
	if (**input == '0')
	{
		(*input)++;
		return (ft_strdup("minishell"));
	}
	return (ft_expand_normal_var(input, state, start));
}

char	*ft_my_getenv(char *var_name, t_shell_state *state)
{
	t_env	*current;
	current = state->env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (getenv(var_name));
}
