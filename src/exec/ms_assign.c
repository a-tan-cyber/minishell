/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:52:59 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 15:53:14 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_assign_ident(const char *arg, size_t *eq)
{
	size_t	i;

	if (!arg || !arg[0] || !(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	if (arg[i] != '=')
		return (0);
	if (eq)
		*eq = i;
	return (1);
}

static int	ms_assign_one(t_info *i, const char *arg)
{
	size_t	eq;
	char	*name;
	int		ret;

	if (!ms_assign_ident(arg, &eq))
		return (1);
	name = ft_substr(arg, 0, eq);
	if (!name)
		return (1);
	ret = ms_var_set(&i->vars, name, arg + eq + 1, FALSE);
	ft_sfree((void **)&name);
	return (ret);
}

static int	ms_assign_all(t_info *i, char **argv)
{
	int	j;

	j = 0;
	while (argv[j])
	{
		if (ms_assign_one(i, argv[j]))
			return (1);
		j++;
	}
	return (ms_var_sync_env(i));
}

int	ms_try_assign_only(t_info *i, t_ast *cmd)
{
	int	j;

	if (!i || !cmd || !cmd->args || !cmd->args[0] || cmd->rdir)
		return (0);
	j = 0;
	while (cmd->args[j])
	{
		if (!ms_assign_ident(cmd->args[j], NULL))
			return (0);
		j++;
	}
	i->err = ms_assign_all(i, cmd->args);
	return (1);
}
