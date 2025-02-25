/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:06:39 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 17:48:30 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

typedef struct s_pipex
{
	int		pipe_fds[2];
	int		infile;
	int		outfile;
	char	*outfile_path;
	char	*cmd1;
	char	*cmd2;
	char	**cmd_paths;
	char	**envp;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

// Initialize the pipex structure
void	init_pipex(t_pipex *pipex, char **argv, char **envp);

// Execute the pipex command
int		exec_pipex(t_pipex *pipex);
void	exec_cmd(char *cmd, char **envp);

// Find the full path of the command
char	*find_path(char *cmd, char **envp);

// Error handling
void	ft_pipex_error(const char *msg, int exit_code);
void	cmd_error(const char *cmd);

// Split the command
int		count_cmds(char *cmd);
char	*extract_quoted(char *cmd, int *i, char quote);
char	*extract_str(char *cmd, int *i);
char	**split_cmd(char *cmd);

#endif
