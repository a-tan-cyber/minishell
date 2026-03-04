/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_wc_match.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 15:23:36 by amtan             #+#    #+#             */
/*   Updated: 2026/03/04 15:51:24 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	wc_key(size_t i, size_t j, size_t lt)
{
	return (i * (lt + 1) + j);
}

static t_bool	wc_rec(t_wc_ctx *c, size_t i, size_t j)
{
	size_t	k;

	k = wc_key(i, j, c->lt);
	if (c->memo[k] != -1)
		return ((t_bool)c->memo[k]);
	if (c->wc[i] == 127 && c->wc[i + 1] == '\0')
		return (c->memo[k] = 1);
	if (c->wc[i] == '\0' && c->th[j] == '\0')
		return (c->memo[k] = 1);
	if (c->wc[i] == 127)
	{
		if (wc_rec(c, i + 1, j) || (c->th[j] && wc_rec(c, i, j + 1)))
			return (c->memo[k] = 1);
		return (c->memo[k] = 0);
	}
	if (c->wc[i] && c->wc[i] == c->th[j] && wc_rec(c, i + 1, j + 1))
		return (c->memo[k] = 1);
	return (c->memo[k] = 0);
}

t_bool	cmp_wc(const char *wc, const char *thing, int c)
{
	signed char	*memo;
	size_t		lw;
	size_t		lt;
	t_wc_ctx	ctx;
	t_bool		res;

	if (c != 127)
		return (FALSE);
	lw = ft_strlen(wc);
	lt = ft_strlen(thing);
	memo = (signed char *)malloc((lw + 1) * (lt + 1));
	if (!memo)
		return (FALSE);
	ft_memset(memo, -1, (lw + 1) * (lt + 1));
	ctx.wc = wc;
	ctx.th = thing;
	ctx.memo = memo;
	ctx.lt = lt;
	res = wc_rec(&ctx, 0, 0);
	free(memo);
	return (res);
}
