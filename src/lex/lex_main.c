/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:40:38 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/24 18:40:38 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//$
size_t	tokenise_text(char *line, t_token **lexed, char **env, t_info *info)
{
	size_t	r;
	char	*text;
	
	r = 0;
	while (line[r] && !is_operator(line[r]) && !ft_is_white_space(line[r])
			&& line[r] != '\'' && line[r] != '\"' && !is_bracket(line[r]))
		r++;
	text = ft_strndup(line, r);
	if (!text)
		return (0);
	if (expand_dollar(&text, env, info) == 0)
		return (ft_sfree((void **)&text), 0);
	ft_str_replace_chr(text, '*', 127);
	if (token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r);
}

int	lex_line(const char *line, t_token **lexed, const char **env, t_info *info)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (line[i])
	{
		if (ft_is_white_space(line[i]))
			len = tokenise_space(line + i, lexed);
		else if (line[i] == '\'')
			len = tokenise_s_quote(line + i, lexed);
		else if (line[i] == '\"')
			len = tokenise_d_quote(line + i, lexed, env, info);
		else if (is_operator(line[i]))
			len = tokenise_oper(line + i, lexed);
		else if (is_bracket(line[i]))
			len = tokenise_brkt(line + i, lexed);
		else
			len = tokenise_text(line + i, lexed, env, info);
		if (len == 0)
			return (free_token_lst(lexed), -1);
		i += len;
	}
	return (merge_and_expand_tokens(lexed));
}

// size_t	skip_whitespace(const char *line)
// {
// 	size_t	i;

// 	i = 0;
// 	while (line[i] && ft_is_white_space(line[i]))
// 	{
// 		i++;
// 	}
// 	return (i);
// }
// int	is_oper_token(t_oper type)
// {
// 	if (type == PIPE || type == REDI_IN || type == REDI_OT || type == APPEND
// 		|| type == HEREDOC || type == AND || type == OR)
// 		return (1);
// 	return (0);
// }
