/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_mode.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 12:24:12 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 14:45:11 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static void	add_prog_name(t_lexer *lexer, char *s)
{
	t_list	*lst;

	if (s)
	{
		if (!(lst = ft_lstnew(s, ft_strlen(s) + 1)))
			ft_error_msg("Malloc failed");
		lst->next = lexer->tokens[0];
		lexer->tokens[0] = lst;
		lexer->count++;
	}
}

static void	clear_data(t_lexer *lexer, char **input, char ***av)
{
	ft_strdel(input);
	reset_lexer(lexer);
	ft_free_string_tab(av);
}

int			interactive_mode(char **argv)
{
	t_lexer	lexer;
	char	*input;
	char	**av;

	init_lexer(&lexer);
	while (42)
	{
		write(STDOUT_FILENO, "Ft_SSL> ", 8);
		if (get_next_line(STDIN_FILENO, &input) < 0)
			ft_error_msg("ft_ssl: Reading user input failed");
		if (!input)
			break ;
		if (lexer_input(&lexer, input))
			continue ;
		add_prog_name(&lexer, argv[0]);
		if (!(av = lst_to_tab(lexer.tokens[0], lexer.count + 1)))
			ft_error_msg("Malloc failed");
		if (av[1] && (!ft_strcmp("exit", av[1]) || !ft_strcmp("quit", av[1])))
			break ;
		ft_ssl_routine(av);
		clear_data(&lexer, &input, &av);
	}
	clear_data(&lexer, &input, &av);
	ft_strdel(&lexer.current_token);
	return (0);
}
