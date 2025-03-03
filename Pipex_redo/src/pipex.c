/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/03/02 14:01:39 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	check_path(char **envp);
static void	exec_pipe(t_pipex *pipex, char **argv, char **envp);
static void	first_child(t_pipex *pipex, char **argv, char **envp);
static void	second_child(t_pipex *pipex, char **argv, char **envp);

// Main pipex structure
	// Check the PATH
	// 	
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	check_path(envp);
	if (argc == 5)
	{
		exec_pipe(&pipex, argv, envp);
		return (EXIT_SUCCESS);
	}
	else
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}

// Get the directories in the PATH environment variable
	// If the environment variable is not set, use the default paths
	// Find the PATH environment variable
	// Split the PATH environment variable by ':' and return the directories
static void	check_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return ;
		i++;
	}
	ft_putstr_fd("pipex: No environment PATH found.\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

// Execute the pipe
static void	exec_pipe(t_pipex *pipex, char **argv, char **envp)
{	
	int		status1;
	int		status2;

	if (pipe(pipex->pipe_fds) == -1)
		exit_error("pipe failed");
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		exit_error("fork 1 failed");
	if (pipex->pid1 == 0)
		first_child(pipex, argv, envp);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		exit_error("fork 2 failed");
	if (pipex->pid2 == 0)
		second_child(pipex, argv, envp);
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	waitpid(pipex->pid1, &status1, 0);
	waitpid(pipex->pid2, &status2, 0);
	if (WIFEXITED(status2))
		exit(WEXITSTATUS(status2));
	else
		exit(EXIT_FAILURE);
}

// First child process
	// Close the read end of the pipe, not used by this child process
	// Redirect the standard input to the input file
	// Redirect the standard output to the write end of the pipe
	// Close the original descriptors after redirection
	// Execute the first command
static void	first_child(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
		put_error(argv[1]);
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		exit_error("dup2 pipe write error");
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		exit_error("dup2 infile error");
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
		exit_error(argv[4]);
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		exit_error("dup2 pipe read error");
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		exit_error("dup2 outfile error");
	close(pipex->pipe_fds[1]);
	exec_cmd(argv[3], envp, pipex);
}
