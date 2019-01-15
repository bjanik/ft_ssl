/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:31:54 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/15 11:31:55 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"
#include "libft.h" 
#include <string.h>

void		realloc_current_token(t_lexer *lexer)
{
	char	*tmp;

	tmp = lexer->current_token;
	lexer->token_size *= 2;
	if (!(lexer->current_token = (char *)ft_memalloc((lexer->token_size + 1)
			* sizeof(char))))
		ft_error_msg("Malloc failed\n");
	// ft_bzero(lexer->current_token, lexer->token_size + 1);
	ft_strcpy(lexer->current_token, tmp);
	ft_strdel(&tmp);
}

static void		skip_char(t_lexer *lexer)
{
	(void)lexer;
}

void		append_char(t_lexer *lexer)
{
	if (lexer->token_len == lexer->token_size)
		realloc_current_token(lexer);
	lexer->current_token[lexer->token_len++] = *(lexer->input);
}

static void		get_event(t_lexer *lexer)
{
	char	c;

	c = *(lexer->input);
	if (c == '"')
		lexer->event = EV_DQUOTE;
	else if (c == '\'')
		lexer->event = EV_QUOTE;
	else if (ft_isspace(c))
		lexer->event = EV_BLANK;
	else if (c != '\0')
		lexer->event = EV_REG_CHAR;
}

void		del(void *content, size_t size)
{
	(void)size;
	ft_memdel(&content);
}

void			lexer_input(t_lexer *lexer, char *input)
{
	lexer->input = input;
	lexer->event = 0;
	while (*(lexer->input) != '\0')
	{
		g_lexer[lexer->state][lexer->event].p_transition(lexer);
		if (lexer->state != INIT)
			lexer->input++;
		lexer->state = g_lexer[lexer->state][lexer->event].new_state;
		get_event(lexer);
	}
	delimitate_token(lexer);
	lexer->input = NULL;
}

const t_transition		g_lexer[MAX_STATE][MAX_EVENT] = {
	{{STD, skip_char},
		{DQUOTE, append_char},
		{QUOTE, append_char},
		{STD, append_char},
		{STD, append_char}},	

	{{DQUOTE, append_char},
		{STD, skip_char},
		{DQUOTE, append_char},
		{DQUOTE, append_char},
		{DQUOTE, append_char}},

	{{QUOTE, append_char},
		{QUOTE, append_char},
		{STD, skip_char},
		{QUOTE, append_char},
		{QUOTE, append_char}},

	{{STD, append_char},
		{DQUOTE, skip_char},
		{QUOTE, skip_char},
		{STD, delimitate_token},
		{STD, append_char}},
};