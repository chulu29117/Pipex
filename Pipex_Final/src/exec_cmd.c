/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/03/03 11:55:05 by clu              ###   ########.fr       */
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
		exit(exit_code);
	if (execve(path, args, envp) == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free(path);
		ft_free_array(args);
		exit(127);
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
		ft_pipex_error("pipex: memory allocation failed", 1);
	if (!(*args)[0])
	{
		ft_free_array(*args);
		cmd_error(cmd);
		return (127);
	}
	*path = find_path((*args)[0], envp);
	if (!*path)
	{
		ft_free_array(*args);
		if (ft_strchr(cmd, '/') != NULL || cmd[0] == '.')
		{
			ft_putstr_fd("pipex: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		}
		else
			cmd_error(cmd);
		return (127);
	}
	return (0);
}
