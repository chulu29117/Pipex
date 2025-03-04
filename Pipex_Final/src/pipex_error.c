/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:32 by clu               #+#    #+#             */
/*   Updated: 2025/03/04 23:31:09 by clu              ###   ########.fr       */
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

int	valid_cmd(char *cmd)
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
