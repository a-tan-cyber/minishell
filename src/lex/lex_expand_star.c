/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expand_star.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:25 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/03 23:56:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// expand all stars (DEL char / 127). 0 if success
// int	expand_star(char *line, t_token **lexed, char **env, t_info *info)
int	expand_star(const char *line, t_token **lexed)
{
	t_bool			ins;
	DIR				*dir;
	struct dirent	*f;
	t_token			*tmp;

	dir = opendir(".");
	if (!dir)
		return (1);
	ins = FALSE;
	f = readdir(dir);
	while (f)
	{
		if ((((f->d_name)[0] == '.' && line[0] == '.') || f->d_name[0] != '.')
			&& cmp_wc(line, f->d_name, 127) && !ins_wct(f->d_name, lexed, &ins))
			return (closedir(dir), 2);
		f = readdir(dir);
	}
	if (ins == FALSE)
		return (ft_str_replace_chr((*lexed)->text, 127, '*'), closedir(dir), 0);
	tmp = *lexed;
	*lexed = (*lexed)->prev;
	return (free_token_one(&tmp), closedir(dir), 0);
}

t_bool	cmp_wc_rec(const char *wc, const char *thing, size_t i, size_t j)
{
	if (wc[i] == 127 && wc[i + 1] == '\0')
		return (TRUE);
	else if (wc[i] == '\0' && thing[j] == '\0')
		return (TRUE);
	if (wc[i] == 127)
	{
		if (cmp_wc_rec(wc, thing, i + 1, j) == TRUE)
			return (TRUE);
		if (thing[j] && cmp_wc_rec(wc, thing, i, j + 1) == TRUE)
			return (TRUE);
		return (FALSE);
	}
	if (wc[i] == thing[j])
		return (cmp_wc_rec(wc, thing, i + 1, j + 1));
	return (FALSE);
}

t_bool	cmp_wc(const char *wc, const char *thing, int c)
{
	t_bool	res;

	if (c == 127)
		res = cmp_wc_rec(wc, thing, 0, 0);
	else
		res = FALSE;
	return (res);
}

//insert a token before the current token
t_token	*ins_wct(char *text, t_token **cur, t_bool *inserted)
{
	char	*new_text;
	t_token	*new_token;

	new_text = ft_strdup(text);
	if (!new_text)
		return (free(new_text), NULL);
	new_token = ins_token_front(cur, TEXT, new_text);
	if (new_token == NULL)
		return (NULL);
	*inserted = TRUE;
	return (new_token);
}
