/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/28 14:08:53 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Prepare the command for execution
static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	// Split the command into an array of arguments
	*args = split_cmd(cmd);
	if (!*args)
		ft_pipex_error("pipex: memory allocation failed", 1);
	// If the first argument is NULL, free the array and exit
	if (!(*args)[0])
	{
		ft_free_array(*args);
		cmd_error(cmd);
		exit (127);
	}
	// Find the full path of the command
	*path = find_path((*args)[0], envp);
	if (!*path)
	{
		ft_free_array(*args);
		cmd_error(cmd);
		exit (127);
	}
	return (0);
}

// Execute the command
void	exec_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*path;
	int		exit_code;

	// Prepare the command (split arguments and find full path)
	exit_code = prepare_cmd(cmd, &args, &path, envp);
	if (exit_code != 0)
		exit(exit_code);
	// Execute the command
	if (execve(path, args, envp) == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free(path);
		ft_free_array(args);
		// If the command is not found, exit with status 127
		if (errno == ENOENT)
			ft_pipex_error(args[0], 127);
		// If the permission is denied, exit with status 126
		else if (errno == EACCES)
			ft_pipex_error(args[0], 126);
		else
			exit(1);
	}
}
