/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 10:10:16 by clu               #+#    #+#             */
/*   Updated: 2025/02/03 12:04:40 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	if (!envp || !*envp)
		return (NULL);
	while (*envp && ft_strncmp(*envp, "PATH=", 5) != 0)
		envp++;
	if (!*envp)
		return (NULL);
	paths = ft_split(*envp + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		if (paths[i][ft_strlen(paths[i]) - 1] == '/')
			full_path = ft_strjoin(paths[i], cmd);
		else
		{
			path = ft_strjoin(paths[i], "/");
			full_path = ft_strjoin(path, cmd);
			free(path);
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}
