/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:49:39 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 15:40:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_dollar(char **text, t_info *info)
{
	size_t	i;

	if (!text || !*text)
		return (0);
	i = 0;
	while ((*text)[i])
	{
		if ((*text)[i] == '$')
		{
			i += expand_dollar_replace(text, i, info);
			if (!*text)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

size_t	expand_dollar_replace(char **text, size_t i, t_info *info)
{
	char	*snippet;
	char	*expanded;
	char	*temp;
	size_t	len;

	snippet = expand_dollar_extract(*text + i, &len);
	if (!snippet)
		return (ft_sfree((void **)text), 0);
	if (ft_strcmp(snippet, "$") == 0)
		return (ft_sfree((void **)&snippet), 1);
	expanded = match_env(snippet, info);
	ft_sfree((void **)&snippet);
	if (!expanded)
		return (ft_sfree((void **)text), 0);
	temp = ft_str_replace(*text, i, len, expanded);
	len = ft_strlen(expanded);
	ft_sfree((void **)&expanded);
	ft_sfree((void **)text);
	if (!temp)
		return (0);
	*text = temp;
	return (len);
}

static void	set_sizet(size_t *num, size_t val)
{
	*num = val;
}

char	*expand_dollar_extract(char *line, size_t *len)
{
	size_t	i;

	*len = 0;
	if (!line || !line[0] || line[0] != '$')
		return (NULL);
	i = 1;
	if (line[i] == '{')
	{
		while (line[i] && line[i] != '}')
			i++;
		if (!line[i])
			return (NULL);
		return (set_sizet(len, i + 1), ft_strndup(line + 2, i - 2));
	}
	else if (line[i] == '?')
		return (set_sizet(len, i + 1), ft_strdup("?"));
	else if (ft_isalnum(line[i]) || line[i] == '_')
	{
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		return (set_sizet(len, i), ft_strndup(line + 1, i - 1));
	}
	return (set_sizet(len, 1), ft_strndup(line, 1));
}
