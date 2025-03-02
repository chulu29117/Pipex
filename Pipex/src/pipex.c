/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/02/28 15:02:29 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Get the directories in the PATH environment variable
	// If the environment variable is not set, use the default paths
	// Find the PATH environment variable
	// Split the PATH environment variable by ':' and return the directories
static void	check_path(char **envp)
{
	int	i;
	int	valid;

	i = 0;
	valid = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			valid = 1;
		i++;
	}
	if (!valid)
	{
		put_error("No environment PATH found.");
		exit(1);
	}
}

// First child process
	// Close the read end of the pipe, not used by this child process
	// Redirect the standard input to the input file
	// Redirect the standard output to the write end of the pipe
	// Close the original descriptors after redirection
	// Execute the first command
static void	first_child(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY, 0777);
	if (pipex->infile == -1)
		exit_error();
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		exit_error();
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		exit_error();
	close(pipex->pipe_fds[0]);
	exec_cmd(argv[2], envp, pipex);
}

// Second child process
	// Close the write end of the pipe
	// Redirect the standard input to the read end of the pipe
	// Open the output file for writing	
	// Redirect the standard output to the output file
	// Execute the second command
static void	second_child(t_pipex *pipex, char **argv, char **envp)
{
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		exit_error();
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		exit_error();
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		exit_error();
	close(pipex->pipe_fds[1]);
	exec_cmd(argv[3], envp, pipex);
}

// Check number of arguments
	// Initialize the pipex structure
	// Execute the pipex command
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	check_path(envp);
	if (argc == 5)
	{
		if (pipe(pipex.pipe_fds) == -1)
			exit_error();
		pipex.pid = fork();
		if (pipex.pid == -1)
			exit_error();
		if (pipex.pid == 0)
			first_child(&pipex, argv, envp);
		waitpid(pipex.pid, NULL, 0);
		second_child(&pipex, argv, envp);
	}
	else
	{
		put_error("Usage: ./pipex file1 cmd1 cmd2 file2");
		exit(EXIT_FAILURE);
	}
}
