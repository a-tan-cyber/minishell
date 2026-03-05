/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 01:03:54 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 01:03:58 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ms_env_make_kv(const char *key, const char *val)
{
	char	*tmp;
	char	*kv;

	if (!key || !val)
		return (NULL);
	tmp = ft_strjoin2(key, "=");
	if (!tmp)
		return (NULL);
	kv = ft_strjoin2(tmp, val);
	ft_sfree((void **)&tmp);
	return (kv);
}

static int	ms_env_find(char **env, const char *key, size_t *idx)
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

static int	ms_env_append(char ***envp, char *kv)
{
	char	**env;
	char	**newenv;
	size_t	len;
	size_t	i;

	env = *envp;
	len = ft_arrlen((const char **)env);
	newenv = ft_calloc(len + 2, sizeof(char *));
	if (!newenv)
		return (1);
	i = 0;
	while (i < len)
	{
		newenv[i] = env[i];
		i++;
	}
	newenv[len] = kv;
	free(env);
	*envp = newenv;
	return (0);
}

char	*ms_env_get(char **env, const char *key)
{
	size_t	k;
	size_t	i;

	if (!env || !key)
		return (NULL);
	k = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, k) && env[i][k] == '=')
			return (env[i] + k + 1);
		i++;
	}
	return (NULL);
}

int	ms_env_set(char ***envp, const char *key, const char *val)
{
	char	*kv;
	size_t	idx;

	if (!envp || !key || !val)
		return (1);
	kv = ms_env_make_kv(key, val);
	if (!kv)
		return (1);
	if (ms_env_find(*envp, key, &idx))
	{
		ft_sfree((void **)&(*envp)[idx]);
		(*envp)[idx] = kv;
		return (0);
	}
	if (ms_env_append(envp, kv))
		return (ft_sfree((void **)&kv), 1);
	return (0);
}
