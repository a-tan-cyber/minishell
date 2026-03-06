/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:17:53 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 12:32:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_env_find_idx(char **env, const char *key, size_t *idx)
{
	size_t	k;
	size_t	i;

	if (!env || !key || !idx)
		return (0);
	k = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, k) && env[i][k] == '=')
			return (*idx = i, 1);
		i++;
	}
	return (0);
}

static void	ms_env_unset_at(char **env, size_t idx)
{
	size_t	i;

	free(env[idx]);
	i = idx;
	while (env[i + 1])
	{
		env[i] = env[i + 1];
		i++;
	}
	env[i] = NULL;
}

int	ms_builtin_unset(t_info *i, char **argv)
{
	int		j;
	size_t	idx;

	if (!i || !argv)
		return (1);
	j = 1;
	while (argv[j])
	{
		if (ms_env_find_idx(i->my_env, argv[j], &idx))
			ms_env_unset_at(i->my_env, idx);
		j++;
	}
	return (0);
}
