/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/03/02 23:39:06 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check number of arguments
	// Initialize the pipex structure
	// Execute the pipex command
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc == 5)
		return (exec_pipex(&pipex, argv, envp));
	else
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
