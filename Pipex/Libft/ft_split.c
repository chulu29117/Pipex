/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 20:51:38 by clu               #+#    #+#             */
/*   Updated: 2024/12/22 15:55:58 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c);
static char		*get_next_word(char const *s, char c, size_t *i);
static int		write_split(char **split, char const *s, char c);

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	if (!split)
		return (NULL);
	if (!write_split(split, s, c))
	{
		ft_free_split(split);
		return (NULL);
	}
	return (split);
}

static size_t	count_words(char const *s, char c)
{
	size_t	words;
	size_t	i;

	words = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			words++;
		i++;
	}
	return (words);
}

static char	*get_next_word(char const *s, char c, size_t *i)
{
	size_t	start;
	size_t	end;

	start = *i;
	while (s[start] && s[start] == c)
		start++;
	if (!s[start])
		return (NULL);
	end = start;
	while (s[end] && s[end] != c)
		end++;
	*i = end;
	return (ft_substr(s, start, end - start));
}

void	ft_free_split(char **split)
{
	size_t	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static int	write_split(char **split, char const *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
		{
			split[j] = get_next_word(s, c, &i);
			if (split[j] == NULL)
			{
				ft_free_split(split);
				return (0);
			}
			j++;
		}
		else
			i++;
	}
	split[j] = NULL;
	return (1);
}
