/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:59:38 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 18:30:56 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	here_doc(char *limiter)
{
	char	*line;
	size_t	len;
	ssize_t	nread;
	int		tmp_fd;

	line = NULL;
	len = 0;
	tmp_fd = open("here_doc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (tmp_fd < 0)
		ft_pipex_error("Error creating here_doc tmp file", 1);
	while (1)
	{
		write(1, "heredoc> ", 9);
		nread = getline(&line, &len, stdin);
		if (nread <= 0)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 &&
			line[ft_strlen(limiter)] == '\n')
			break ;
		write(tmp_fd, line, nread);
	}
	free(line);
	close(tmp_fd);
	tmp_fd = open("here_doc_tmp", O_RDONLY);
	if (tmp_fd < 0)
		ft_pipex_error("Error reopening here_doc tmp file", 1);
	return (tmp_fd);
}
