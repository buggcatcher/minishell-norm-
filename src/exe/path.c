#include "minishell.h"

char	**get_paths(t_env *env)
{
	char	*path_var;
	path_var = get_env_value("PATH", env);
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

char	*search_in_paths(char **paths, char *cmd)
{
	size_t	i;
	char	*full_path;
	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}