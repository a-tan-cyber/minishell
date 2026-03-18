/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_expand_star.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 18:13:25 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/18 10:43:45 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wc_free(char **m, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		free(m[i++]);
	free(m);
}

static int	wc_add_sorted(char ***m, size_t *n, size_t *cap, const char *name)
{
	char	**new_m;
	char	*s;
	size_t	pos;

	if (*n == *cap)
	{
		new_m = (char **)malloc(sizeof(char *) * ((*cap * 2) + 1));
		if (!new_m)
			return (1);
		ft_memcpy(new_m, *m, sizeof(char *) * (*n));
		free(*m);
		*m = new_m;
		*cap *= 2;
	}
	s = ft_strdup(name);
	if (!s)
		return (1);
	pos = (*n)++;
	while (pos > 0 && ft_strcmp((*m)[pos - 1], s) > 0)
	{
		(*m)[pos] = (*m)[pos - 1];
		pos--;
	}
	(*m)[pos] = s;
	return (0);
}

static char	**wc_collect(const char *line, DIR *dir, size_t *n)
{
	struct dirent	*f;
	char			**m;
	size_t			cap;

	*n = 0;
	cap = 16;
	m = (char **)malloc(sizeof(char *) * (cap + 1));
	if (!m)
		return (NULL);
	f = readdir(dir);
	while (f)
	{
		if (((f->d_name[0] == '.' && line[0] == '.') || f->d_name[0] != '.')
			&& cmp_wc(line, f->d_name, 127)
			&& wc_add_sorted(&m, n, &cap, f->d_name))
			return (wc_free(m, *n), NULL);
		f = readdir(dir);
	}
	m[*n] = NULL;
	return (m);
}

static int	wc_insert_tokens(char **m, size_t n, t_token **lexed, char *raw)
{
	size_t	i;
	t_token	*tok;

	i = 0;
	while (i < n)
	{
		tok = ins_token_front(lexed, TEXT, m[i]);
		if (!tok)
			return (free(m[i]), 1);
		if (raw)
		{
			tok->raw = ft_strdup(raw);
			if (!tok->raw)
			{
				m[i] = NULL;
				free_token_one(&tok);
				return (1);
			}
		}
		m[i++] = NULL;
	}
	return (0);
}

int	expand_star(const char *line, t_token **lexed)
{
	DIR		*dir;
	char	**m;
	size_t	n;
	t_token	*tmp;

	dir = opendir(".");
	if (!dir)
		return (1);
	m = wc_collect(line, dir, &n);
	if (!m)
		return (closedir(dir), 2);
	if (n == 0)
		return (ft_str_replace_chr((*lexed)->text, 127, '*'),
			wc_free(m, n), closedir(dir), 0);
	if (wc_insert_tokens(m, n, lexed, (*lexed)->raw))
		return (wc_free(m, n), closedir(dir), 2);
	tmp = *lexed;
	return (wc_free(m, n), *lexed = (*lexed)->prev,
		free_token_one(&tmp), closedir(dir), 0);
}
