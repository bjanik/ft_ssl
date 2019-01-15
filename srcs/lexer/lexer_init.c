/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 11:26:30 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/15 11:26:31 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "lexer.h"

void		init_lexer(t_lexer *lexer)
{

	if (!(lexer->current_token = (char*)ft_memalloc(INITIAL_TOKEN_SIZE + 1)))
		ft_error_msg("Malloc failed\n");
	lexer->input = NULL;
	ft_bzero(lexer->current_token, INITIAL_TOKEN_SIZE + 1);
	lexer->token_size = INITIAL_TOKEN_SIZE;
	lexer->token_len = 0;
	lexer->state = 0;
	lexer->event = START;
	lexer->tokens[0] = NULL;
	lexer->tokens[1] = NULL;
	lexer->count = 0;
}