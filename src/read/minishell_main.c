/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 08:29:03 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/17 11:33:31 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// astree = parse_ast(i.lexed);

void	set_sizet(size_t *num, size_t val)
{
	*num = val;
}

char	*expand_dollar_extract(char *line, size_t *len)
{
	size_t	i;

	*len = 0;
	if (!line || !line[0] || line[0] != '$')
		return (NULL);
	i = 1;
	if (line[i] == '{')
	{
		while (line[i] && line[i] != '}')
			i++;
		if (!line[i])
			return (NULL);
		return (set_sizet(len, i + 1), ft_strndup(line + 2, i - 2));
	}
	else if (line[i] == '?')
		return (set_sizet(len, i + 1), ft_strdup("?"));
	else if (ft_isalnum(line[i]) || line[i] == '_')
	{
		while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
			i++;
		return (set_sizet(len, i), ft_strndup(line + 1, i - 1));
	}
	return (set_sizet(len, 1), ft_strndup(line, 1));
}

// return index if present, -1 if not present
int	ft_envcmp(char *str, char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(str, arr[i], ft_strlen(str)) == 0
			&& arr[i][ft_strlen(str)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char *ft_extract_env(int i, char **arr)
{
	int	j;
	
	j = 0;
	while (arr[i][j] && arr[i][j] != '=')
		j++;
	return (ft_strdup(arr[i] + j + 1));
}

// ?; number
char	*match_env(char *snippet, char **env, t_info *info)
{
	if (ft_strcmp(snippet, "?") == 0)
		return (ft_itoa(info->err));
	else if (ft_str_is_num(snippet) == 1)
		return (ft_strdup(""));
	else if (ft_envcmp(snippet, env) != -1)
		return (ft_extract_env(ft_envcmp(snippet, env), env));
	return (ft_strdup(""));
}

char	*ft_str_replace(char *text, size_t start, size_t len, char *expanded)
{
	size_t	total_len;
	size_t	expanded_len;
	char	*res;

	expanded_len = ft_strlen(expanded);
	total_len = ft_strlen(text) - len + expanded_len;
	res = malloc((total_len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	ft_memcpy(res, text, start);
	ft_memcpy(res + start, expanded, expanded_len);
	ft_strcpy(res + start + expanded_len, text + start + len);
	return (res);
}

size_t	expand_dollar_replace(char **text, size_t i, char **env, t_info *info)
{
	char	*snippet;
	char	*expanded;
	char	*temp;
	size_t	len;

	snippet = expand_dollar_extract(*text + i, &len);
	if (!snippet)
		return (ft_sfree(text), 0);
	if (ft_strcmp(snippet, "$") == 0)
		return (ft_sfree(&snippet), 1);
	expanded = match_env(snippet, env, info);
	ft_sfree(&snippet);
	if (!expanded)
		return (ft_sfree(text), 0);
	temp = ft_str_replace(*text, i, len, expanded);
	len = ft_strlen(expanded);
	ft_sfree(&expanded);
	ft_sfree(text);
	if (!temp)
		return (0);
	*text = temp;
	return (len);
}

int	expand_dollar(char **text, char **env, t_info *info)
{
	size_t	i;

	if (!text ||!*text)
		return (0);
	i = 0;
	while ((*text)[i])
	{
		if ((*text)[i] == '$')
		{
			i += expand_dollar_replace(text, i, env, info);
			if (!*text)
				return (0);
		}
		else
			i++;
	}
	return (1);
}

size_t	tokenise_d_quote(const char *line, t_token **lexed, const char **env,
	t_info *info)
{
	size_t	l;
	size_t	r;
	char	*text;

	l = 1;
	r = l;
	while (line[r] && line[r] != '\"')
		r++;
	if (line[r] == '\0')
		return (0);
	text = ft_strndup(line + l, r - l);
	expand_dollar(&text, env, info);
	if (!text)
		return (0);
	if (token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r + 1);
}

size_t	tokenise_space(char *line, t_token **lexed)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_is_white_space(line[i]))
		i++;
	if (token_push_back(lexed, SPCE, NULL) == NULL)
		return (0);
	return (i);
}

size_t	tokenise_oper_or(t_token **lexed)
{
	if (token_push_back(lexed, OR, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_pipe(t_token **lexed)
{
	if (token_push_back(lexed, PIPE, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_and(t_token **lexed)
{
	if (token_push_back(lexed, AND, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_heredoc(t_token **lexed)
{
	if (token_push_back(lexed, HEREDOC, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediin(t_token **lexed)
{
	if (token_push_back(lexed, REDI_IN, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_append(t_token **lexed)
{
	if (token_push_back(lexed, APPEND, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediot(t_token **lexed)
{
	if (token_push_back(lexed, REDI_OT, NULL) == NULL)
		return (0);
	return (1);
}

size_t	tokenise_oper(char *line, t_token **lexed)
{
	size_t	res;

	res = 1;
	if (line[0] == '|' && line[1] == '|')
		res = tokenise_oper_or(lexed);
	else if (line[0] == '|')
		res = tokenise_oper_pipe(lexed);
	else if (line[0] == '&' && line[1] == '&')
		res = tokenise_oper_and(lexed);
	else if (line[0] == '<' && line[1] == '<')
		res = tokenise_oper_heredoc(lexed);
	else if (line[0] == '<')
		res = tokenise_oper_rediin(lexed);
	else if (line[0] == '>' && line[1] == '>')
		res = tokenise_oper_append(lexed);
	else if (line[0] == '>')
		res = tokenise_oper_rediot(lexed);
	return (res);
}

size_t	tokenise_brkt(char *line, t_token **lexed)
{
	if (line[0] == '(')
	{
		if (token_push_back(lexed, PAREN_L, NULL) == NULL)
			return (0);
		return (1);
	}
	else if (line[0] == ')')
	{
		if (token_push_back(lexed, PAREN_R, NULL) == NULL)
			return (0);
		return (1);
	}
	return (0);
}

//$
size_t	tokenise_text(char *line, t_token **lexed, char **env, t_info *info)
{
	size_t	r;
	char	*text;
	
	r = 0;
	while (line[r] && !is_operator(line[r]) && !ft_is_white_space(line[r])
			&& line[r] != '\'' && line[r] != '\"' && !is_bracket(line[r]))
		r++;
	text = ft_strndup(line, r);
	if (!text)
		return (0);
	if (expand_dollar(&text, env, info) == 0)
		return (ft_sfree((void **)&text), 0);
	if (token_push_back(lexed, TEXT, text) == NULL)
		return (ft_sfree((void **)&text), 0);
	return (r);
}

int	expand_star(char *line, t_token **lexed, char **env, t_info *info)
{
	size_t	i;

	i = 0;
	if (line[i])
	{
		
	}
}

// *; success returns 0, failure returns -1;
int	merge_tokens(t_token **lexed)
{
	t_token	*cur;

	cur = *lexed;
	while (cur)
	{
		
	}
	
	// expand_star()
	return (0);
}

int	lex_line(const char *line, t_token **lexed, const char **env, t_info *info)
{
	size_t	i;
	size_t	len;

	i = 0;
	while (line[i])
	{
		if (ft_is_white_space(line[i]))
			len = tokenise_space(line + i, lexed);
		else if (line[i] == '\'')
			len = tokenise_s_quote(line + i, lexed);
		else if (line[i] == '\"')
			len = tokenise_d_quote(line + i, lexed, env, info);
		else if (is_operator(line[i]))
			len = tokenise_oper(line + i, lexed);
		else if (is_bracket(line[i]))
			len = tokenise_brkt(line + i, lexed);
		else
			len = tokenise_text(line + i, lexed, env, info);
		if (len == 0)
			return (free_token_lst(lexed), -1);
		i += len;
	}
	return (merge_tokens(lexed));
}
	
int	main(int argc, char **argv, char **envp)
{
	t_ast	*astree;
	t_info	i;

	init_ms_var(&astree, &i);
	signal(SIGINT, int_handler);
	signal(SIGQUIT, SIG_IGN);
	if (argc != 1 || ft_arrdup_mk2(&i.my_env, envp) != 0)
		return (free_ms_var(&astree, &i, "all"), 2);
	while (TRUE)
	{
		i.line = read_multiline("moonshell> ");
		if (!i.line || !ft_strcmp(i.line, "exit\n"))
			return (free_ms_var(&astree, &i, "all"), 0);
		if (line_valid(&i.line))
		{
			add_history(i.line);
			if (lex_line(i.line, &i.lexed, i.my_env, &i) == 0)
				astree = parse_ast(i.lexed);
			if (astree)
				run_cmd(astree, i);
		}
		free_ms_var(&astree, &i, "tmp");
	}
	return (free_ms_var(&astree, &i, "all"), 0);
}
// int	main(int argc, char **argv, char **envp)
// {
// 	t_ast	*astree;
// 	char	**my_env;
// 	char	*line;
// 	char	*lexed;

// 	nullise_var(&astree, &my_env, &line, &lexed);
// 	if (argc != 1 || ft_arrdup_mk2(&my_env, envp) != 0)
// 		return (free_null_var(NULL, &my_env, NULL, NULL), 2);
// 	while (TRUE)
// 	{
// 		line = read_multiline("moonshell> ");
// 		//if line has error then freenullvar then exit.
// 		if (!line || ft_strcmp(line, "exit") == 0)
// 			return (free_null_var(&astree, &my_env, &line, &lexed), 0);
// 		if (line != NULL)
// 			add_history(line);
// 		if (line && lex_line(line, &lexed) == 0)
// 		{
// 			astree = build_ast(lexed);
// 			if (astree)
// 				run_cmd(astree);
// 		}
// 		free_null_var(&astree, NULL, &line, &lexed);
// 	}
// 	return (free_null_var(&astree, &my_env, NULL, NULL), 0);
// }
