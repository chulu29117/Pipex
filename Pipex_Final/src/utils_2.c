/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:09:56 by clu               #+#    #+#             */
/*   Updated: 2025/03/08 21:10:00 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_exit_buffer(char *buffer, int exit_code)
{
	free(buffer);
	ft_pipex_error("pipex: malloc failed", exit_code);
}

void	close_fd(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	ft_pipex_error("pipex: fork failed", 1);
}
