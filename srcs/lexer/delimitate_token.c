/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimitate_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 12:14:53 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/16 18:07:09 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static t_list	*init_token_node(t_lexer *lexer)
{
	t_list	*token;

	if (!(token = ft_lstnew(lexer->current_token, lexer->token_len + 1)))
		ft_error_msg("Malloc failed\n");
	ft_bzero(lexer->current_token, lexer->token_len);
	lexer->token_len = 0;
	lexer->count++;
	return (token);
}

void			delimitate_token(t_lexer *lexer)
{
	t_list	*token;

	if (lexer->token_len > 0)
	{
		token = init_token_node(lexer);
		if (!lexer->tokens[0])
		{
			lexer->tokens[0] = token;
			lexer->tokens[1] = token;
		}
		else
		{
			lexer->tokens[1]->next = token;
			lexer->tokens[1] = token;
		}
	}
}
