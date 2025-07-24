#include "minishell.h"

int	ft_check_dquote(t_shell_state *state, t_token *token, char *start)
{
	int	i;
	(void)token;
	i = 0;
	while (start[i] && start[i] != '"')
		i++;
	if (start[i] != '"')
	{
		state->last_status = 2;
		ft_putstr_stderr("Unclosed double quote\n");
		return (1);
	}
	return (0);
}

int	ft_check_var(char **input)
{
	char	*tmp;
	int		start_pos;
	if (**input != '$')
		return (1);	
	tmp = *input + 1;
	start_pos = 0;
	if (tmp[start_pos] == '?')
		return (2);
	while (tmp[start_pos] && \
			((tmp[start_pos] >= '0' && tmp[start_pos] <= '9') || \
			(tmp[start_pos] >= 'A' && tmp[start_pos] <= 'Z') || \
			(tmp[start_pos] >= 'a' && tmp[start_pos] <= 'z') || \
			tmp[start_pos] == '_' || tmp[start_pos] == '$'))
		start_pos++;
	if (start_pos == 0)
		return (1);
	else
		return (2);
}

char	*ft_create_var(char *buffer, char **input, t_shell_state *state)
{
	char	*var;
	char	*tmp;
	if (**input == '$')
	{
		var = ft_expand_var(input, state);
		if (!buffer)
			buffer = var;
		else
		{
			tmp = ft_strjoin(buffer, var);
			free(buffer);
			free(var);
			buffer = tmp;
		}
	}
	else
	{
		buffer = ft_append_char(buffer, **input);
		(*input)++;
	}
	return (buffer);
}