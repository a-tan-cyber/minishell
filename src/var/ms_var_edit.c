/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_var_edit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:11:28 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 15:11:30 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_var	*ms_var_find(t_var *vars, const char *name)
{
	while (vars)
	{
		if (!ft_strcmp(vars->name, name))
			return (vars);
		vars = vars->next;
	}
	return (NULL);
}

char	*ms_var_get(t_var *vars, const char *name)
{
	t_var	*var;

	if (!name)
		return (NULL);
	var = ms_var_find(vars, name);
	if (!var)
		return (NULL);
	return (var->value);
}

int	ms_var_set(t_var **vars, const char *name, const char *value,
		t_bool exported)
{
	t_var	*var;
	char	*dup;

	if (!vars || !name)
		return (1);
	var = ms_var_find(*vars, name);
	if (!var)
	{
		var = ms_var_new(name, value, exported);
		if (!var)
			return (1);
		return (ms_var_add_back(vars, var), 0);
	}
	if (value)
	{
		dup = ft_strdup(value);
		if (!dup)
			return (1);
		ft_sfree((void **)&var->value);
		var->value = dup;
	}
	if (exported)
		var->exported = TRUE;
	return (0);
}

void	ms_var_unset(t_var **vars, const char *name)
{
	t_var	*cur;
	t_var	*prev;

	if (!vars || !name)
		return ;
	prev = NULL;
	cur = *vars;
	while (cur && ft_strcmp(cur->name, name))
	{
		prev = cur;
		cur = cur->next;
	}
	if (!cur)
		return ;
	if (!prev)
		*vars = cur->next;
	else
		prev->next = cur->next;
	ft_sfree((void **)&cur->name);
	ft_sfree((void **)&cur->value);
	ft_sfree((void **)&cur);
}

int	ms_var_sync_env(t_info *i)
{
	char	**env;

	if (!i)
		return (1);
	env = ms_var_build_env(i->vars);
	if (!env)
		return (1);
	free_arr(i->my_env);
	i->my_env = env;
	return (0);
}
