/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokenise.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 21:43:24 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/25 21:43:24 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	tokenise_s_quote(const char *line, t_token **lexed)
{
	size_t	l;
	size_t	r;
	char	*text;

	l = 1;
	r = l;
	while (line[r] && line[r] != '\'')
	{
		r++;
	}
	if (line[r] == '\0')
		return (0);
	text = ft_strndup(line + l, r - l);
	if (!text || token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r + 1);
}