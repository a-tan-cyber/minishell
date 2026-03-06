/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export_print.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 13:55:01 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 15:14:52 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*ms_export_line(t_var *var)
{
	char	*tmp;
	char	*line;

	tmp = ft_strjoin2("declare -x ", var->name);
	if (!tmp)
		return (NULL);
	if (!var->value)
		return (tmp);
	line = ft_strjoin2(tmp, "=\"");
	ft_sfree((void **)&tmp);
	if (!line)
		return (NULL);
	tmp = ft_strjoin2(line, var->value);
	ft_sfree((void **)&line);
	if (!tmp)
		return (NULL);
	line = ft_strjoin2(tmp, "\"");
	ft_sfree((void **)&tmp);
	return (line);
}

static char	**ms_export_copy(t_var *vars)
{
	t_var	*cur;
	char	**cpy;
	size_t	i;

	i = 0;
	cur = vars;
	while (cur)
	{
		if (cur->exported)
			i++;
		cur = cur->next;
	}
	cpy = ft_calloc(i + 1, sizeof(char *));
	if (!cpy)
		return (NULL);
	i = 0;
	while (vars)
	{
		if (vars->exported)
			cpy[i++] = ms_export_line(vars);
		if (i && !cpy[i - 1])
			return (free_arr(cpy), NULL);
		vars = vars->next;
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

static void	ms_export_sort(char **arr)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (arr && arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strcmp(arr[i], arr[j]) > 0)
				ms_export_swap(&arr[i], &arr[j]);
			j++;
		}
		i++;
	}
}

void	ms_export_print_all(t_var *vars)
{
	char	**cpy;
	size_t	i;

	cpy = ms_export_copy(vars);
	if (!cpy)
		return ;
	ms_export_sort(cpy);
	i = 0;
	while (cpy[i])
	{
		ft_putendl_fd(cpy[i], STDOUT_FILENO);
		i++;
	}
	free_arr(cpy);
}
