/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/02/23 22:56:38 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Get the directories in the PATH environment variable
static char	**get_path_dirs(char **envp)
{
	int			i;
	char		**paths;
	static char	*default_paths[] = {"/bin", "/usr/bin", NULL};

	// If the environment variable is not set, use the default paths
	if (!envp || !*envp)
		return (default_paths);
	i = 0;
	// Find the PATH environment variable
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (default_paths);
	// Split the PATH environment variable by ':' and return the directories
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (default_paths);
	return (paths);
}

// Check if the command is in the directories in the PATH
static char	*exec_in_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		// temp = directory + '/'
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		// full_path = directory + '/' + command
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		// Check if the command is executable, X_OK mode for executable
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// Find the full path of the command
char	*find_path(char *cmd, char **envp)
{
	char		**paths;
	char		*full_path;

	// If the command is an absolute or relative path
	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	// Get the directories from PATH
	paths = get_path_dirs(envp);
	if (!paths)
		return (NULL);
	// Check if the command is in the directories in the PATH
	full_path = exec_in_path(cmd, paths);
	if (paths != get_path_dirs(NULL))
		ft_free_array(paths);
	return (full_path);
}
