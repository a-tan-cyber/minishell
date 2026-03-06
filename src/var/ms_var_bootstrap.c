/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_var_bootstrap.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:41:13 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 20:12:11 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_var	*ms_var_from_envline(const char *s)
{
	size_t	i;
	char	*name;
	t_var	*var;

	i = 0;
	while (s && s[i] && s[i] != '=')
		i++;
	name = ft_substr(s, 0, i);
	if (!name)
		return (NULL);
	if (s && s[i] == '=')
		var = ms_var_new(name, s + i + 1, TRUE);
	else
		var = ms_var_new(name, NULL, TRUE);
	ft_sfree((void **)&name);
	return (var);
}

static char	*ms_var_make_envline(t_var *var)
{
	size_t	ln;
	size_t	lv;
	char	*line;

	ln = ft_strlen(var->name);
	lv = ft_strlen(var->value);
	line = ft_calloc(ln + lv + 2, sizeof(char));
	if (!line)
		return (NULL);
	ft_memcpy(line, var->name, ln);
	line[ln] = '=';
	ft_memcpy(line + ln + 1, var->value, lv);
	return (line);
}

static t_var	*ms_var_from_envp(char **envp)
{
	t_var	*vars;
	t_var	*new;
	size_t	i;

	vars = NULL;
	i = 0;
	while (envp && envp[i])
	{
		new = ms_var_from_envline(envp[i]);
		if (!new)
			return (ms_var_clear(&vars), NULL);
		ms_var_add_back(&vars, new);
		i++;
	}
	return (vars);
}

char	**ms_var_build_env(t_var *vars)
{
	t_var	*cur;
	char	**env;
	size_t	i;

	i = 0;
	cur = vars;
	while (cur)
	{
		if (cur->exported && cur->value)
			i++;
		cur = cur->next;
	}
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	while (vars)
	{
		if (vars->exported && vars->value)
			env[i++] = ms_var_make_envline(vars);
		if (i && !env[i - 1])
			return (free_arr(env), NULL);
		vars = vars->next;
	}
	return (env);
}

int	ms_var_bootstrap(t_info *i, char **envp)
{
	if (!i)
		return (1);
	i->vars = ms_var_from_envp(envp);
	if (!i->vars && envp && envp[0])
		return (1);
	i->my_env = ms_var_build_env(i->vars);
	if (!i->my_env)
		return (ms_var_clear(&i->vars), 1);
	return (0);
}
