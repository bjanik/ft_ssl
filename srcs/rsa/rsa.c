/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rsa.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:30:51 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:30:53 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bn.h"
#include "ft_ssl.h"

static char			*create_public_key(t_rsa *rsa)
{
	unsigned char	*public_data;
	char			*public_data_encoded;
	uint32_t		public_data_len;
	uint32_t		total_len;

	public_data_len = get_public_data_len(rsa->rsa_data.modulus,
											rsa->rsa_data.public_exp);
	total_len = public_data_len + get_byte_number(public_data_len)
								+ (public_data_len >= 0x80) + 1;
	if ((public_data = (unsigned char*)ft_malloc(total_len)) == NULL)
		return (NULL);
	fill_pem_public_data(public_data,
							public_data_len,
							rsa->rsa_data.modulus,
							rsa->rsa_data.public_exp);
	public_data_encoded = base64_encode_data(public_data, public_data_len);
	ft_memdel((void**)&public_data);
	return (public_data_encoded);
}

static int			private_routine(t_rsa *rsa, char *args[],
										unsigned char **data,
										uint32_t *data_len)
{
	int ret;

	if (rsa->des)
	{
		args[1] = rsa->passin;
		if (!(*data = private_key_decryption(rsa->des, *data, data_len, args)))
			return (1);
	}
	ret = parse_decoded_data(&rsa->rsa_data, *data, *data_len, rsa->opts);
	if (rsa->opts & RSA_TEXT)
		flag_text(rsa);
	if (rsa->opts & RSA_MODULUS)
		flag_modulus(rsa->rsa_data.modulus, rsa->fd[OUT]);
	if (rsa->opts & RSA_CHECK)
		ret = flag_check(rsa->rsa_data);
	if ((rsa->opts & RSA_DES) && (rsa->opts & RSA_PUBOUT) == 0)
	{
		reset_des(rsa->des);
		rsa->des = init_des("des-cbc", g_commands[6].des_mode);
		args[1] = rsa->passout;
		if (!(*data = private_key_encryption(rsa->des, *data, data_len, args)))
			return (1);
	}
	return (0);
}

int					rsa_private_key_routine(t_rsa *rsa, char **data)
{
	unsigned char	*raw_data;
	uint32_t		raw_data_len;
	char			*args[2];

	args[0] = rsa->in;
	raw_data_len = 0;
	raw_data = base64_decode_data(&raw_data_len,
									ft_strdup(*data),
									ft_strlen(*data));
	if (raw_data == NULL)
		return (1);
	if (private_routine(rsa, args, &raw_data, &raw_data_len))
		return (1);
	ft_strdel(data);
	*data = base64_encode_data(raw_data, raw_data_len);
	ft_memdel((void**)&raw_data);
	return (0);
}

static int			rsa_handle_key(t_rsa *rsa, char **data)
{
	if (rsa->opts & RSA_PUBIN)
	{
		if ((*data = get_data(rsa->fd[IN], &rsa->des, PEM_PUBLIC_HEADER,
								PEM_PUBLIC_FOOTER)) == NULL)
			return (1);
		if (rsa->opts & RSA_CHECK)
		{
			ft_dprintf(STDERR_FILENO, "Only private keys can be checked\n");
			ft_strdel(data);
			return (1);
		}
		if (retrieve_data_from_public_key(&rsa->rsa_data, *data))
			return (1);
		if (rsa->opts & RSA_TEXT)
			flag_text(rsa);
		if (rsa->opts & RSA_MODULUS)
			flag_modulus(rsa->rsa_data.modulus, rsa->fd[OUT]);
	}
	else if (rsa->opts & RSA_PUBOUT)
	{
		if ((*data = create_public_key(rsa)) == NULL)
			return (1);
	}
	return (0);
}

int					rsa_command_run(t_rsa *rsa)
{
	char			*data;
	int				ret;

	if (rsa_input_file(rsa))
		return (1);
	if ((rsa->opts & RSA_PUBIN) == 0)
	{
		if ((data = get_data(rsa->fd[IN], &rsa->des, PEM_PRIVATE_HEADER,
								PEM_PRIVATE_FOOTER)) == NULL)
			return (1);
		if (rsa_private_key_routine(rsa, &data) || data == NULL)
		{
			ft_strdel(&data);
			return (1);
		}
	}
	ret = rsa_handle_key(rsa, &data);
	rsa_output_file(rsa);
	if ((rsa->opts & RSA_NOOUT) == 0)
		print_rsa_key(rsa, data, rsa->fd[OUT]);
	ft_strdel(&data);
	return (ret);
}
