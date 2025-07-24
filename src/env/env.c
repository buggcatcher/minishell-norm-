#include "minishell.h"

void	set_env_var(t_env **env, char *key, char *value)
{
	t_env	*cur;
	t_env	*new;
	cur = *env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			cur->value = ft_strdup_m(value);
			return ;
		}
		cur = cur->next;
	}
	new = safe_alloc(1, sizeof(t_env), "env_set_new_node");
	if (!new)
		return ;
	new->key = ft_strdup_m(key);
	new->value = ft_strdup_m(value);
	new->exported = 1;
	new->next = *env;
	*env = new;
}

t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

t_env	*create_env_node(char *key, char *value, int exported)
{
	t_env	*new_node;
	new_node = safe_alloc(1, sizeof(t_env), "create_env_node");
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	new_node->exported = exported;
	if (value)
		new_node->value = ft_strdup(value);
	else
		new_node->value = NULL;
	if (!new_node->key || (value && !new_node->value))
	{
		free(new_node->key);
		if (value) free(new_node->value);
		free(new_node);
		return (NULL);
    }
	new_node->next = NULL;
	return (new_node);
}

int	remove_env_node(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
