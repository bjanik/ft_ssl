
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