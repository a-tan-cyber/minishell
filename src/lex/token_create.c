/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 08:26:27 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/07 17:15:08 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*token_create_ele(t_oper type, char *text)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->text = text;
	new->quoted = FALSE;
	new->hd_delim = FALSE;
	new->line_no = 0;
	new->prev = NULL;
	new->next = NULL;
	return (new);
}

static t_token	*token_push_front(t_token **begin_list, t_oper type, char *text)
{
	t_token	*first;

	first = token_create_ele(type, text);
	if (!first)
		return (NULL);
	first->next = *begin_list;
	if (*begin_list)
		(*begin_list)->prev = first;
	*begin_list = first;
	return (first);
}

t_token	*token_last(t_token *begin_list)
{
	t_token	*res;

	res = begin_list;
	while (begin_list)
	{
		res = begin_list;
		begin_list = begin_list->next;
	}
	return (res);
}

t_token	*token_push_back(t_token **begin_list, t_oper type, char *text)
{
	t_token	*last;
	t_token	*temp;

	if (*begin_list == NULL)
	{
		temp = token_push_front(begin_list, type, text);
		return (temp);
	}
	else
	{
		temp = token_last(*begin_list);
		last = token_create_ele(type, text);
		if (!last)
			return (NULL);
		temp->next = last;
		last->prev = temp;
	}
	return (last);
}

t_token	*ins_token_front(t_token **head, t_oper type, char *text)
{
	t_token	*needle;
	t_token	*tempor;

	if (!(*head) || (*head)->prev == NULL)
	{
		needle = token_push_front(head, type, text);
		if (!needle)
			return (NULL);
		*head = (*head)->next;
		return (needle);
	}
	needle = token_create_ele(type, text);
	if (!needle)
		return (NULL);
	tempor = (*head)->prev;
	(*head)->prev = needle;
	needle->prev = tempor;
	tempor->next = needle;
	needle->next = *head;
	return (needle);
}
