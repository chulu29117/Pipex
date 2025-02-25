/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_quoted.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 21:01:54 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 21:02:05 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check if the character is a quote character
static void	update_quote(char c, char *quote, int *in_string, int *cmd_count)
{
	if (*quote == 0 && (c == '\'' || c == '\"'))
		*quote = c;
	else if (*quote == c)
		*quote = 0;
	if (!ft_is_whitespace(c) || quote != 0)
	{
		if (in_string == 0)
		{
			cmd_count++;
			*in_string = 1;
		}
	}
	else
		*in_string = 0;
}

// Count the number of commands in the command string
int	count_cmds(char *cmd)
{
	int		i;
	int		cmd_count;
	int		in_string;
	char	quote;

	i = 0;
	cmd_count = 0;
	in_string = 0;
	quote = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && cmd[i + 1])
		{
			i += 2;
			continue ;
		}
		update_quote(cmd[i], &quote, &in_string, &cmd_count);
		i++;
	}
	return (cmd_count);
}
