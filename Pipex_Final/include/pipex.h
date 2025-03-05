/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:06:39 by clu               #+#    #+#             */
/*   Updated: 2025/03/05 13:27:25 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <errno.h>
# include "libft.h"

// Structure for pipex
typedef struct s_pipex
{
	int		pipe_fds[2];
	int		infile;
	int		outfile;
	char	*cmd_path;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

// Structure for buffer for substrings
typedef struct s_buffer
{
	char	*result;
	int		pos;
	int		capacity;
}	t_buffer;

// Initialize the pipex structure
// void	init_pipex(t_pipex *pipex, char **argv, char **envp);

// Execute the pipex command
int		exec_pipex(t_pipex *pipex, char **argv, char **envp);
void	exec_cmd(char *cmd, char **envp);

// Find the full path of the command
char	*find_path(char *cmd, char **envp);

// Error handling
void	ft_pipex_error(const char *msg, int exit_code);
void	cmd_error(const char *cmd);
int		valid_cmd(char *cmd);
void	execve_error(char *cmd);
char	*check_cmd_errno(char *full_path, int *errno_flag);

// Split the command
int		count_cmds(char *cmd);
char	*extract_str(char *cmd, int *i);
char	**split_cmd(char *cmd);

#endif
