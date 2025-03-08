/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/03/08 21:16:19 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check number of arguments
	// Initialize the pipex structure
	// Execute the pipex command
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		exit_status;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_bzero(&pipex, sizeof(pipex));
	exit_status = exec_pipex(&pipex, argv, envp);
	return (exit_status);
}
