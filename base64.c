/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/08 14:17:27 by bjanik            #+#    #+#             */
/*   Updated: 2018/11/10 17:50:26 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

#define B64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define QUANTUM_SIZE 3

#define IN 0
#define OUT 1

#define ENCODE 0
#define DECODE 1

/**
** By default, option -e is set. If -e option and -d option are both set, last option set is considered
** Need to know where read from (input file or STDIN)
** If no input file, read from STDIN
** if no output file, write on STDOUT
** Encoding data processing works on 3 bytes, translated into 4 groups of 6 bits (meaning 4 bytes with 64 as max value)
** in case of infinite data (f. eg. cat /dev/random | ./ft_ssl base64 ) we need to output encoded data blocks by blocks
** ==> Store encoded data in a buffer, buffer grows at every encoding iteration. When full, output buffer's content
** If in decode mode and input has characters not belonging to the base64 alphabet, abort processing an output nothing
**/

// Assume option are set

void		init_processing(t_base64 *base)
{
	base->fd[IN] = STDIN_FILENO;
	base->fd[OUT] = STDOUT_FILENO;
	if (base->input_file)
		if ((base->fd[IN] = open(base->input_file, 0755, O_RDONLY)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
	if (base->output_file)
		if ((base->fd[OUT] = open(base->output_file, 0755, O_WRONLY |
													O_CREAT |
													O_TRUNC)) < 0)
		{
			ft_putstr_fd("ft_ssl: ", STDERR_FILENO);
			perror(base->input_file);
		}
}

void		encode_data(t_base64 *base, char b[])
{
	base->encoded[0] = (b[0] >> 2) & 0xFF;
	base->encoded[1] = ((b[0] & 0x03) << 4) | ((b[1] >> 4) & 0xFF);
	base->encoded[2] = ((b[1] & 0x0F) << 2) | ((b[2] >> 6) & 0xFF);
	base->encoded[3] = b[2] & 0x3F;
}

void		decode_data(t_base64 *base, char b[])
{
	base->decoded[0] = (b[0] << 2) | ((b[1] & 0x30) >> 4);
	base->decoded[1] = ((b[1] & 0x0F) << 4) | (b[2] >> 2);
	base->decoded[2] = ((b[2] & 0x03) << 6) | b[3];
}

void		put_encoded_data_to_buffer(t_base64 *base,
										char buffer[],
										int *buflen,
										int ret)
{
	int	i;

	i = -1;
	while (++i < 4)
	{
		if (ret < QUANTUM_SIZE && ret < i && 
			base->encoded[i] == 0)
			buffer[*buflen + i] = '=';
		else
			buffer[*buflen + i] = B64[base->encoded[i]];
	}
	*buflen += 4;
}

void		get_data(t_base64 *base, uint8_t code)
{
	int		ret;
	int 	buflen;
	char	buffer[BUF_SIZE + 1];
	char	*b;
	void	(*data[2])(t_base64 *base, char *b) = {
		encode_data,
		decode_data,
	};

	if (!(b = ft_memalloc((QUANTUM_SIZE + code))))
		return ;
	buflen = 0;
	ft_memset(buffer, 0x0, BUF_SIZE + 1);
	while ((ret = read(base->fd[IN], b, QUANTUM_SIZE + code)) > 0)
	{
		data[code](base, b);
		put_encoded_data_to_buffer(base, buffer, &buflen, ret);
		if (buflen == BUF_SIZE)
		{
			write(base->fd[OUT], buffer, buflen);
			ft_memset(buffer, 0x0, BUF_SIZE + 1);
			buflen = 0;
		}
		ft_memset(b, 0x0, QUANTUM_SIZE);
	}
	write(base->fd[OUT], buffer, buflen);
}

int			main(int argc, char **argv)
{
	t_base64 base;

	base.input_file = argv[1];
	base.output_file = argv[2];
	init_processing(&base);
	get_data(&base, ENCODE);
	ft_putchar('\n');
}
