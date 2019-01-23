/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reset_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 11:51:48 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 11:52:23 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	reset_lexer(t_lexer *lexer)
{
	lexer->token_len = 0;
	ft_bzero(lexer->current_token, lexer->token_size + 1);
	lexer->count = 0;
	lexer->state = INIT;
	lexer->event = START;
	ft_lstdel(&lexer->tokens[0], del);
	lexer->tokens[1] = NULL;
}
