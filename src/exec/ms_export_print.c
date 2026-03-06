/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:55:01 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 13:55:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ms_export_copy(char **env)
{
	size_t	len;
	size_t	i;
	char	**cpy;

	len = ft_arrlen((const char **)env);
	cpy = ft_calloc(len + 1, sizeof(char *));
	if (!cpy)
		return (NULL);
	i = 0;
	while (i < len)
	{
		cpy[i] = env[i];
		i++;
	}
	return (cpy);
}

static void	ms_export_swap(char **a, char **b)
{
	char	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	ms_export_sort(char **env)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (env && env[i])
	{
		j = i + 1;
		while (env[j])
		{
			if (ft_strcmp(env[i], env[j]) > 0)
				ms_export_swap(&env[i], &env[j]);
			j++;
		}
		i++;
	}
}

static void	ms_export_print_one(const char *s)
{
	size_t	i;

	if (!s)
		return ;
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	write(STDOUT_FILENO, s, i);
	if (!s[i])
		return (write(STDOUT_FILENO, "\n", 1), (void)0);
	ft_putstr_fd("=\"", STDOUT_FILENO);
	ft_putstr_fd((char *)(s + i + 1), STDOUT_FILENO);
	ft_putendl_fd("\"", STDOUT_FILENO);
}

void	ms_export_print_all(char **env)
{
	char	**cpy;
	size_t	i;

	if (!env)
		return ;
	cpy = ms_export_copy(env);
	if (!cpy)
		return ;
	ms_export_sort(cpy);
	i = 0;
	while (cpy[i])
	{
		ms_export_print_one(cpy[i]);
		i++;
	}
	free(cpy);
}
