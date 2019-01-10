/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/24 12:55:06 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/24 12:55:17 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ft_ssl.h"

void			base64_opts(char **argv, t_base64 *base)
{
	int i;

	i = 1;
	while (argv[++i])
	{
		if (!ft_strcmp("-i", argv[i]))
			base->input_file = argv[++i];
		else if (!ft_strcmp("-o", argv[i]))
			base->output_file = argv[++i];
		else if (!ft_strcmp("-d", argv[i]))
			base->opts |= B_OPT_D;
		else if (!ft_strcmp("-e", argv[i]))
			base->opts &= B_OPT_D;
		else
		{
			ft_memdel((void**)&base);
			ft_error_msg("ft_ssl: base64: invalid option");
		}
	}
	set_base64_fds(base);
}


static void		set_base64_fds(t_base64 *base)
{
	if (base->input_file)
		if ((base->fd[IN] = open(base->input_file, O_RDONLY, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			ft_memdel((void**)&base);
			perror(base->input_file);
		}
	if (base->output_file)
		if ((base->fd[OUT] = open(base->output_file, O_WRONLY |
													O_CREAT |
													O_TRUNC, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			ft_memdel((void**)&base);
			perror(base->input_file);
		}	
}

t_base64		*init_base64(void)
{
	t_base64	*base;

	if (!(base = (t_base64*)malloc(sizeof(t_base64))))
		return (NULL);
	base->fd[IN] = STDIN_FILENO;
	base->fd[OUT] = STDOUT_FILENO;
	ft_memset(base->buffer, 0, BUF_SIZE + 1);
	base->input_file = NULL;
	base->output_file = NULL;
	if (base->input_file)
		if ((base->fd[IN] = open(base->input_file, O_RDONLY, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
	if (base->output_file)
		if ((base->fd[OUT] = open(base->output_file, O_WRONLY |
													O_CREAT |
													O_TRUNC, 0644)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
	return (base);
}

void		init_b64(t_des *des, t_base64 *base)
{
	base->fd[IN] = des->fd[IN];
	base->fd[OUT] = des->fd[OUT];
}