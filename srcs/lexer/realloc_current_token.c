#include "ft_ssl.h"

void			realloc_current_token(t_lexer *lexer)
{
	char	*tmp;

	tmp = lexer->current_token;
	lexer->token_size *= 2;
	if (!(lexer->current_token = (char *)ft_memalloc((lexer->token_size + 1)
			* sizeof(char))))
		ft_error_msg("Malloc failed\n");
	ft_strcpy(lexer->current_token, tmp);
	ft_strdel(&tmp);
}