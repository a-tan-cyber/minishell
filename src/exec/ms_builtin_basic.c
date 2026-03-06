/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_basic.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:17:09 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 12:17:18 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_echo_is_n(const char *s)
{
	int	i;

	if (!s || s[0] != '-')
		return (0);
	i = 1;
	if (s[i] != 'n')
		return (0);
	while (s[i] == 'n')
		i++;
	return (s[i] == '\0');
}

int	ms_builtin_echo(char **argv)
{
	int	nl;
	int	i;

	nl = 1;
	i = 1;
	while (argv[i] && ms_echo_is_n(argv[i]))
	{
		nl = 0;
		i++;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		if (argv[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (nl)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	ms_builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		ft_putstr_fd("moonshell: pwd: ", STDERR_FILENO);
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	return (0);
}

int	ms_builtin_env(t_info *i)
{
	int	j;

	if (!i || !i->my_env)
		return (0);
	j = 0;
	while (i->my_env[j])
	{
		if (ft_strchr(i->my_env[j], '='))
			ft_putendl_fd(i->my_env[j], STDOUT_FILENO);
		j++;
	}
	return (0);
}
