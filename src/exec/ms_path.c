/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 00:05:19 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 00:13:01 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ms_env_value(char **env, const char *key)
{
	size_t	klen;
	int		i;

	if (!env || !key)
		return (NULL);
	klen = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
			return (env[i] + klen + 1);
		i++;
	}
	return (NULL);
}

static char	*ms_join_dir_cmd(const char *dir, const char *cmd)
{
	const char	*base;
	size_t		ld;
	size_t		lc;
	char		*out;

	base = dir;
	if (!base || base[0] == '\0')
		base = ".";
	ld = ft_strlen(base);
	lc = ft_strlen(cmd);
	out = ft_calloc(ld + lc + 2, sizeof(char));
	if (!out)
		return (NULL);
	ft_memcpy(out, base, ld);
	out[ld] = '/';
	ft_memcpy(out + ld + 1, cmd, lc);
	return (out);
}

static int	ms_make_try(const char *path, const char *cmd,
				size_t i, size_t j, char **try, char **fallback)
{
	char	*dir;

	dir = ft_substr(path, i, j - i);
	if (!dir)
		return (ft_sfree((void **)fallback), 1);
	*try = ms_join_dir_cmd(dir, cmd);
	ft_sfree((void **)&dir);
	if (!*try)
		return (ft_sfree((void **)fallback), 1);
	return (0);
}

static char	*ms_pick_from_path(const char *path, const char *cmd)
{
	size_t	i;
	size_t	j;
	char	*try;
	char	*fallback;

	i = 0;
	fallback = NULL;
	while (path[i])
	{
		j = i;
		while (path[j] && path[j] != ':')
			j++;
		if (ms_make_try(path, cmd, i, j, &try, &fallback))
			return (NULL);
		if (access(try, X_OK) == 0)
			return (ft_sfree((void **)&fallback), try);
		if (!fallback && access(try, F_OK) == 0)
			fallback = try;
		else
			ft_sfree((void **)&try);
		if (!path[j])
			break ;
		i = j + 1;
	}
	return (fallback);
}

char	*ms_resolve_path(t_info *i, const char *cmd)
{
	char	*path;

	if (!i || !cmd || cmd[0] == '\0' || ft_strchr(cmd, '/'))
		return (NULL);
	path = ms_env_value(i->my_env, "PATH");
	if (!path || path[0] == '\0')
		return (NULL);
	return (ms_pick_from_path(path, cmd));
}
