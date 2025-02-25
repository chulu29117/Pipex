/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 13:50:10 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Get the directories in the PATH environment variable
	// If the environment variable is not set, use the default paths
	// Find the PATH environment variable
	// Split the PATH environment variable by ':' and return the directories
static char	**get_path_dirs(char **envp)
{
	int			i;
	char		**paths;

	if (!envp || !*envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

// Check if the command is in the directories in the PATH
	// temp = directory + '/'
	// full_path = directory + '/' + command
	// Check if the command is executable, X_OK mode for executable
static char	*exec_in_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

// Find the full path of the command
	// If the command is an absolute or relative path
	// Get the directories from PATH
	// Check if the command is in the directories in PATH
char	*find_path(char *cmd, char **envp)
{
	char		**paths;
	char		*full_path;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (cmd);
		return (NULL);
	}
	paths = get_path_dirs(envp);
	if (!paths)
		return (NULL);
	full_path = exec_in_path(cmd, paths);
	ft_free_array(paths);
	return (full_path);
}
