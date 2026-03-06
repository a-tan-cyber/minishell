/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 00:57:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 15:15:30 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static const char	*ms_cd_target(t_info *i, char **argv, int *print_pwd)
{
	const char	*target;

	*print_pwd = 0;
	if (!argv[1])
	{
		target = ms_var_get(i->vars, "HOME");
		if (!target)
			ft_putendl_fd("moonshell: cd: HOME not set", STDERR_FILENO);
		return (target);
	}
	if (!ft_strcmp(argv[1], "-"))
	{
		target = ms_var_get(i->vars, "OLDPWD");
		if (!target)
			ft_putendl_fd("moonshell: cd: OLDPWD not set", STDERR_FILENO);
		*print_pwd = 1;
		return (target);
	}
	return (argv[1]);
}

static int	ms_cd_fail(const char *target, char **oldpwd)
{
	ft_putstr_fd("moonshell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	ft_sfree((void **)oldpwd);
	return (1);
}

static void	ms_cd_update_env(t_info *i, char *oldpwd, char *newpwd)
{
	if (oldpwd)
		ms_var_set(&i->vars, "OLDPWD", oldpwd, TRUE);
	if (newpwd)
		ms_var_set(&i->vars, "PWD", newpwd, TRUE);
	ms_var_sync_env(i);
}

static int	ms_cd_apply(t_info *i, const char *target, int print_pwd)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
		return (ms_cd_fail(target, &oldpwd));
	newpwd = getcwd(NULL, 0);
	if (print_pwd)
	{
		if (newpwd)
			ft_putendl_fd(newpwd, STDOUT_FILENO);
		else
			ft_putendl_fd((char *)target, STDOUT_FILENO);
	}
	ms_cd_update_env(i, oldpwd, newpwd);
	ft_sfree((void **)&oldpwd);
	ft_sfree((void **)&newpwd);
	return (0);
}

int	ms_builtin_cd(t_info *i, char **argv)
{
	const char	*target;
	int			print_pwd;

	if (!i || !argv || !argv[0])
		return (1);
	if (argv[1] && argv[2])
	{
		ft_putendl_fd("moonshell: cd: too many arguments", STDERR_FILENO);
		return (1);
	}
	target = ms_cd_target(i, argv, &print_pwd);
	if (!target)
		return (1);
	return (ms_cd_apply(i, target, print_pwd));
}
