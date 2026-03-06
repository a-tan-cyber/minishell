/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_export.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:54:01 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 13:57:02 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ms_export_err(const char *arg)
{
	ft_putstr_fd("moonshell: export: `", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static int	ms_export_ident(const char *arg, size_t *eq)
{
	size_t	i;

	if (!arg || !arg[0])
		return (0);
	if (!(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	if (eq)
		*eq = i;
	return (1);
}

static int	ms_export_apply(t_info *i, const char *arg)
{
	size_t	eq;
	char	*key;
	int		ret;

	if (!ms_export_ident(arg, &eq))
		return (ms_export_err(arg), 1);
	if (arg[eq] != '=')
		return (0);
	key = ft_substr(arg, 0, eq);
	if (!key)
		return (1);
	ret = ms_env_set(&i->my_env, key, arg + eq + 1);
	ft_sfree((void **)&key);
	return (ret);
}

int	ms_builtin_export(t_info *i, char **argv)
{
	int	j;
	int	status;

	if (!i || !argv)
		return (1);
	if (!argv[1])
		return (ms_export_print_all(i->my_env), 0);
	status = 0;
	j = 1;
	while (argv[j])
	{
		if (ms_export_apply(i, argv[j]))
			status = 1;
		j++;
	}
	return (status);
}
