/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/03/05 13:29:47 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_path_dirs(char **envp);
static char	*exec_in_path(char *cmd, char **paths);

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
		if (valid_cmd(cmd) != 0)
			return (NULL);
		return (cmd);
	}
	paths = get_path_dirs(envp);
	if (!paths)
	{
		if (valid_cmd(cmd) != 0)
			return (NULL);
		return (cmd);
	}
	full_path = exec_in_path(cmd, paths);
	ft_free_array(paths);
	return (full_path);
}

// Get the directories in the PATH environment variable
	// If the environment variable is not set, return NULL
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
	// Check if the command exists and is executable
static char	*exec_in_path(char *cmd, char **paths)
{
	char	*full_path;
	char	*temp;
	int		i;
	int		errno_flag;

	i = 0;
	errno_flag = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (check_cmd_errno(full_path, &errno_flag))
			return (full_path);
		free(full_path);
		i++;
	}
	errno = errno_flag;
	return (NULL);
}
