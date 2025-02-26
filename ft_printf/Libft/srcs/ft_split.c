/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 19:31:17 by mteichma          #+#    #+#             */
/*   Updated: 2024/11/21 22:52:31 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	free_words(char **words, size_t i)
{
	while (i > 0)
	{
		free(words[i - 1]);
		i--;
	}
	free(words);
}

static size_t	count_words(const char *s, char c)
{
	size_t	count;
	size_t	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*allocate_word(const char *s, char c, size_t *start)
{
	size_t	len;
	char	*word;

	while (s[*start] && s[*start] == c)
		(*start)++;
	len = 0;
	while (s[*start + len] && s[*start + len] != c)
		len++;
	word = (char *)malloc((len + 1) * sizeof(char));
	if (word)
		ft_strlcpy(word, s + *start, len + 1);
	*start += len;
	return (word);
}

static char	**split_and_fill(const char *s, char c, size_t words_count)
{
	char	**words;
	size_t	i;
	size_t	start;

	words = malloc((words_count + 1) * sizeof(char *));
	if (!words)
		return (NULL);
	i = 0;
	start = 0;
	while (i < words_count)
	{
		words[i] = allocate_word(s, c, &start);
		if (!words[i])
		{
			free_words(words, i);
			return (NULL);
		}
		i++;
	}
	words[i] = NULL;
	return (words);
}

char	**ft_split(const char *s, char c)
{
	size_t	words_count;

	if (!s)
		return (NULL);
	words_count = count_words(s, c);
	return (split_and_fill(s, c, words_count));
}
