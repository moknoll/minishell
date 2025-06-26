/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 11:59:19 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/26 03:11:38 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static void	add_operator_token(t_token **tokens, t_operator_info *op, int *i)
{
	t_token_info	info;

	info.value = op->value;
	info.type = op->type;
	info.quote_type = NO_QUOTE;
	info.has_space_before = op->has_space;
	add_token(tokens, &info);
	*i += op->step;
}

static void	init_operator_info(t_operator_info *op, char *value, e_token_type type, int step, int has_space)
{
	op->value = value;
	op->type = type;
	op->step = step;
	op->has_space = has_space;
}

static void	handle_operator(char *input, int *i, t_token **tokens,
																int has_space)
{
	t_operator_info	op;

	if (input[*i] == '<' && input[*i + 1] == '<')
		init_operator_info(&op, "<<", HEREDOC, 2, has_space);
	else if (input[*i] == '>' && input[*i + 1] == '>')
		init_operator_info(&op, ">>", REDIRECT_APPEND, 2, has_space);
	else if (input[*i] == '|')
		init_operator_info(&op, "|", PIPE, 1, has_space);
	else if (input[*i] == '<')
		init_operator_info(&op, "<", REDIRECT_IN, 1, has_space);
	else if (input[*i] == '>')
		init_operator_info(&op, ">", REDIRECT_OUT, 1, has_space);
	else
		return ;
	add_operator_token(tokens, &op, i);
}

static int	check_unclosed_quote(char *input, int *i, char quote_char)
{
	if (!(input[*i] == quote_char))
	{
		printf("Syntax error: unclosed %c-quote\n", quote_char);
		return (0);
	}
	return (1);
}

static void	add_quoted_token(char *raw, int has_space, e_quote_type quote_type,
															t_token **tokens)
{
	char			*word;
	t_token_info	info;

	word = ft_strtrim(raw, " \t\n\r\v\f");
	info.value = word;
	info.type = WORD;
	info.quote_type = quote_type;
	info.has_space_before = has_space;
	add_token(tokens, &info);
	free(word);
}

static int	parse_quoted_word(char *input, int *i, t_token **tokens,
																int has_space)
{
	int				start;
	char			quote_char;
	e_quote_type	quote_type;
	char			*raw;

	quote_char = input[*i];
	if (quote_char == '\'')
		quote_type = SINGLE_QUOTE;
	else
		quote_type = DOUBLE_QUOTE;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!check_unclosed_quote(input, i, quote_char))
		return (0);
	raw = ft_strndup(&input[start], *i - start);
	add_quoted_token(raw, has_space, quote_type, tokens);
	free(raw);
	(*i)++;
	return (1);
}

static void	add_unquoted_token(char *word, int has_space, t_token **tokens)
{
	t_token_info	info;

	info.value = word;
	info.type = WORD;
	info.quote_type = NO_QUOTE;
	info.has_space_before = has_space;
	add_token(tokens, &info);
	free(word);
}

static void	parse_unquoted_word(char *input, int *i, t_token **tokens,
																int has_space)
{
	int		start;
	char	*word;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_operator(input[*i])
		&& input[*i] != '\'' && input[*i] != '"')
		(*i)++;
	if (*i > start)
	{
		word = ft_strndup(&input[start], *i - start);
		add_unquoted_token(word, has_space, tokens);
	}
}

static int	handle_word(char *input, int *i, t_token **tokens, int has_space)
{
	if (input[*i] == '\'' || input[*i] == '"')
		return (parse_quoted_word(input, i, tokens, has_space));
	parse_unquoted_word(input, i, tokens, has_space);
	return (1);
}

static void	handle_space(const char *input, int *i, int *has_space_before)
{
	(void)input;
	(*i)++;
	*has_space_before = 1;
}

static int	handle_token(const char *input, int *i, t_token **tokens,
														int *has_space_before)
{
	if (ft_isspace(input[*i]))
		handle_space(input, i, has_space_before);
	else if (is_operator(input[*i]))
	{
		handle_operator((char *)input, i, tokens, *has_space_before);
		*has_space_before = 0;
	}
	else
	{
		if (!handle_word((char *)input, i, tokens, *has_space_before))
			return (0);
		*has_space_before = 0;
	}
	return (1);
}

t_token	*tokenizer(char *input)
{
	t_token	*tokens;
	int		i;
	int		has_space_before;

	has_space_before = 1;
	i = 0;
	tokens = NULL;
	while (input[i])
	{
		if (!handle_token(input, &i, &tokens, &has_space_before))
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}

char	**tokens_to_argv(t_token **token)
{
	char	**argv;
	int		i;

	argv = malloc(sizeof(char *) * (token_list_size(token) + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (*token && (*token)->type != PIPE)
	{
		if ((*token)->type >= REDIRECT_IN && (*token)->type <= HEREDOC)
		{
			*token = (*token)->next;
			if (!*token || (*token)->type != WORD)
				return (NULL);
			argv[i++] = ft_strdup((*token)->value);
			*token = (*token)->next;
			continue ;
		}
		*token = (*token)->next;
	}
	argv[i] = NULL;
	return (argv);
}

int	token_list_size(t_token **token)
{
	int	size;

	size = 0;
	while (token && *token)
	{
		size++;
		token = &((*token)->next);
	}
	return (size);
}
