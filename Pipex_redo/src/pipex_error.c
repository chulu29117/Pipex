/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:32 by clu               #+#    #+#             */
/*   Updated: 2025/03/02 14:01:07 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Exit with error
void	exit_error(const char *msg)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (msg && msg[0] != '\0')
		ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
	exit(EXIT_FAILURE);
}

void	put_error(const char *msg)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	if (msg && msg[0] != '\0')
		ft_putstr_fd(msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	perror("");
}

// Print command not found error message
void	cmd_error(const char *cmd)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
}
