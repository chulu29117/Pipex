/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:32 by clu               #+#    #+#             */
/*   Updated: 2025/02/24 10:59:39 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Print error message and exit
void	ft_pipex_error(const char *msg, int exit_code)
{
	// if (errno == 0)
	// 	errno = exit_code;
	perror(msg);
	exit(exit_code);
}

// Print command not found error message
void	cmd_error(const char *cmd)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	ft_putstr_fd("\n", 2);
}
