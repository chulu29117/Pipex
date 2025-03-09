/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/03/09 11:54:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp);

// Execute the command
	// Prepare the command (split arguments and find full path)
	// Execute the command
	// If the command is not found, exit with status 127
	// If the permission is denied, exit with status 126
void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;
	int		exit_code;

	exit_code = prepare_cmd(cmd, &args, &path, envp);
	if (exit_code != 0)
	{
		ft_free_array(args);
		if (exit_code == 127 && ft_strchr(cmd, '/') == NULL)
			cmd_error(cmd);
		else
			ft_pipex_error(cmd, exit_code);
	}
	if (execve(path, args, envp) == -1)
	{
		free(path);
		ft_free_array(args);
		execve_error(cmd);
		exit(1);
	}
}

// Prepare the command for execution
	// Split the command into an array of arguments
	// If the first argument is NULL, free the array and exit
	// Find the full path of the command
static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	*args = split_cmd(cmd);
	if (!*args)
	{
		ft_putstr_fd("pipex: memory allocation failed", STDERR_FILENO);
		return (1);
	}
	if (!(*args)[0])
		return (127);
	if (ft_strchr((*args)[0], '/') != NULL || (*args)[0][0] == '.')
		*path = ft_strdup((*args)[0]);
	else
		*path = find_path((*args)[0], envp);
	if (!*path)
	{
		if (errno == EACCES)
			return (126);
		else if (errno == ENOENT)
			return (127);
		return (127);
	}
	return (0);
}
