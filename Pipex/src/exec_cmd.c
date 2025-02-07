/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:51 by clu               #+#    #+#             */
/*   Updated: 2025/02/07 16:08:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Prepare the command for execution
// Split the command into arguments
// Find the full path of the command
static int	prepare_cmd(char *cmd, char ***args, char **path, char **envp)
{
	*args = split_cmd(cmd);
	if (!*args)
		pipex_error("pipex: memory allocation failed", 1);
	if (!(*args)[0])
	{
		ft_free_array(*args);
		cmd_error(cmd);
		exit (127);
	}
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
// Exit with an error message if the command cannot be prepared
// If the command cannot be executed, print an error message
// Errno is set to ENOENT if the command is not found
// Errno is set to EACCES if the permission is denied
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
		if (errno == ENOENT)
			pipex_error(args[0], 127);
		else if (errno == EACCES)
			pipex_error(args[0], 126);
		else
			exit(1);
	}
}
