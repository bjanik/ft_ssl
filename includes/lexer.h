/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:19:04 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/15 11:19:05 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# define INITIAL_TOKEN_SIZE 128
# include "libft.h"

enum				e_state
{
	INIT,
	DQUOTE,
	QUOTE,
	STD,
	MAX_STATE,
};

enum				e_event
{
	START,
	EV_DQUOTE,
	EV_QUOTE,
	EV_BLANK,
	EV_REG_CHAR,
	MAX_EVENT,
};

typedef struct		s_lexer
{
	char			*input;
	char			*current_token;
	int				token_len;
	int				token_size;
	int				state;
	int				event;
	size_t 			count;
	t_list			*tokens[2];
}					t_lexer;

typedef struct		s_transition
{
	int				new_state;
	void			(*p_transition)(t_lexer *lexer);
}					t_transition;

void				init_lexer(t_lexer *lexer);
void				delimitate_token(t_lexer *lexer);
void				realloc_current_token(t_lexer *lexer);
void				lexer_input(t_lexer *lexer, char *input);
void				del(void *content, size_t size);


const t_transition	g_lexer[MAX_STATE][MAX_EVENT];
#endif