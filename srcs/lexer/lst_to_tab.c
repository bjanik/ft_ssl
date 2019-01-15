/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_to_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 14:31:28 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/15 14:31:29 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char		**lst_to_tab(t_list *tokens, int count)
{
	t_list	*tk;
	char	**tab;
	int		i;

	i = 0;
	tk = tokens;
	if (!(tab = (char**)malloc(sizeof(char*) * (count + 1))))
		ft_error_msg("Malloc failed\n");
	while (tk)
	{
		if (!(tab[i++] = ft_strdup(tk->content)))
			ft_error_msg("Malloc failed\n");
		tk = tk->next;
	}
	tab[i] = NULL;
	return (tab);
}
