#include "minishell.h"

static int	exported_env_to_array(t_env *env, t_env ***array, int *count)
{
    t_env	*cur;
    int		i;

    *count = 0;
    cur = env;
    while (cur)
    {
        if (cur->exported)
            (*count)++;
        cur = cur->next;
    }
    *array = safe_alloc(*count, sizeof(t_env *), "export_env_array");
    if (!*array)
        return (1);
    i = 0;
    cur = env;
    while (cur)
    {
        if (cur->exported)
            (*array)[i++] = cur;
        cur = cur->next;
    }
    return (0);
}

static void	sort_exported_env_array(t_env **array, int count)
{
    int		i;
    int		j;
    t_env	*temp;

    i = 0;
    while (i < count - 1)
    {
        j = 0;
        while (j < count - i - 1)
        {
            if (ft_strcmp(array[j]->key, array[j + 1]->key) > 0)
            {
                temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

static void	print_exported_env_array(t_env **array, int count)
{
    int	i = 0;

    while (i < count)
    {
        if (array[i]->value)
            printf("declare -x %s=\"%s\"\n", array[i]->key, array[i]->value);
        else
            printf("declare -x %s\n", array[i]->key);
        i++;
    }
}

int	print_exported_env(t_env *env)
{
    t_env	**array;
    int		count;

    if (exported_env_to_array(env, &array, &count))
        return (1);
    sort_exported_env_array(array, count);
    print_exported_env_array(array, count);
    free(array);
    return (0);
}