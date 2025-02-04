/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 23:45:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char **get_path_dirs(char **envp)
{
	int i;
	char **paths;

	if (!envp || !*envp)
		return (NULL);
	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	return (paths);
}

// char *find_path(char *cmd, char **envp)
// {
// 	char	**paths;
// 	char	*full_path;
// 	char	*temp;
// 	int		i;
// 	static char *default_paths[] = {"/bin", "/usr/bin", NULL};

// 	// If envp is NULL or empty, check absolute paths or common directories
// 	if (!envp || !*envp || !get_path_dirs(envp))
// 	{
// 		if (cmd[0] == '/' || cmd[0] == '.')
// 		{
// 			full_path = ft_strdup(cmd);
// 			if (!full_path)
// 				pipex_error("pipex: memory allocation failed");
// 			return (full_path);
// 		}
// 		// Check manually in /bin and /usr/bin
// 		i = 0;
// 		while (default_paths[i])
// 		{
// 			temp = ft_strjoin(default_paths[i], "/");
// 			if (!temp)
// 				pipex_error("pipex: memory allocation failed");
// 			full_path = ft_strjoin(temp, cmd);
// 			free(temp);
// 			if (!full_path)
// 				pipex_error("pipex: memory allocation failed");
// 			if (access(full_path, X_OK) == 0)
// 				return (full_path);
// 			free(full_path);
// 			i++;
// 		}
// 		return (NULL);
// 	}

// 	// Normal PATH handling
// 	paths = get_path_dirs(envp);
// 	if (!paths)
// 		return (NULL);
// 	i = 0;
// 	while (paths[i])
// 	{
// 		temp = ft_strjoin(paths[i], "/");
// 		if (!temp)
// 			pipex_error("pipex: memory allocation failed");
// 		full_path = ft_strjoin(temp, cmd);
// 		free(temp);
// 		if (!full_path)
// 			pipex_error("pipex: memory allocation failed");
// 		if (access(full_path, X_OK) == 0)
// 		{
// 			ft_free_array(paths);
// 			return (full_path);
// 		}
// 		free(full_path);
// 		i++;
// 	}
// 	ft_free_array(paths);
// 	return (NULL);
// }



char *find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*full_path;
	char	*temp;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));

	paths = get_path_dirs(envp);
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(full_path, X_OK) == 0)
			return (ft_free_array(paths), full_path);
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}
