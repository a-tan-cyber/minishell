/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_export_quote.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 23:55:13 by amtan             #+#    #+#             */
/*   Updated: 2026/03/17 23:55:16 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static size_t	ms_export_q_len(const char *value)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 3;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\')
			len++;
		len++;
		i++;
	}
	return (len);
}

char	*ms_export_quote_value(const char *value)
{
	char	*res;
	size_t	i;
	size_t	j;

	res = malloc((ms_export_q_len(value) + 1) * sizeof(char));
	if (!res)
		return (NULL);
	i = 0;
	j = 0;
	res[j++] = '=';
	res[j++] = '"';
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\')
			res[j++] = '\\';
		res[j++] = value[i++];
	}
	res[j++] = '"';
	res[j] = '\0';
	return (res);
}
