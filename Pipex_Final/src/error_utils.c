/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:32 by clu               #+#    #+#             */
/*   Updated: 2025/03/10 12:47:03 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Print error message and exit
void	ft_pipex_error(const char *msg, int exit_code)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	perror(msg);
	exit(exit_code);
}

// Print command not found error message
void	cmd_error(const char *cmd)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

// Print execve error message
void	execve_error(char *cmd)
{
	if (errno == EACCES)
		ft_pipex_error(cmd, 126);
	else if (errno == ENOENT)
	{
		if (ft_strchr(cmd, '/') == NULL)
			cmd_error(cmd);
		else
			ft_pipex_error(cmd, 127);
	}
}

// Free buffer and exit
void	free_exit_buffer(char *buffer, int exit_code)
{
	free(buffer);
	ft_pipex_error("pipex: malloc failed", exit_code);
}

// Close file descriptors and exit
void	close_fd(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	ft_pipex_error("pipex: fork failed", 1);
}
