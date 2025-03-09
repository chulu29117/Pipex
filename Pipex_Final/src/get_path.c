/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/03/09 12:25:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	**get_path_dirs(char **envp);
static char	*exec_in_path(char *cmd, char **paths);
static int	valid_cmd(char *cmd);
static char	*check_cmd_errno(char *full_path, int *errno_flag);

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
	ft_free_split(paths);
	return (full_path);
}

// Get the directories in the PATH environment variable
	// Find the PATH environment variable that starts with "PATH="
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

// Check if the command exists and is executable
	// If the command does not exist, set errno to ENOENT and return -1
	// If the command is not executable, set errno to EACCES and return -1
static int	valid_cmd(char *cmd)
{
	if (access(cmd, F_OK) != 0)
	{
		errno = ENOENT;
		return (-1);
	}
	if (access(cmd, X_OK) != 0)
	{
		errno = EACCES;
		return (-1);
	}
	return (0);
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

// Check if the command exists and is executable
	// If the command exists and is executable, return the full path
	// If the command does not exist, set errno to ENOENT and return NULL
	// If the command is not executable, set errno to EACCES and return NULL
static char	*check_cmd_errno(char *full_path, int *errno_flag)
{
	if (access(full_path, F_OK) == 0)
	{
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else
			*errno_flag = EACCES;
	}
	else
		*errno_flag = ENOENT;
	return (NULL);
}
