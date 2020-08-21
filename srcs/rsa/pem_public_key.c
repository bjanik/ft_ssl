/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pem_public_key.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 13:05:48 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 13:05:50 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "bn.h"

/*
** Module implements functions that stores modulus and public exponent
** in structured DER format, before base64 encoding
*/

unsigned char	g_magic[] = {
	0x06, 0x09, 0x2A, 0x86, 0x48, 0x86, 0xF7, 0x0D, 0x01, 0x01, 0x01, 0x05, 0x0
};

uint32_t	get_public_data_len(t_bn *modulus, t_bn *public_exp)
{
	uint32_t	len;
	uint32_t	modulus_len;
	uint32_t	public_exp_len;

	modulus_len = bn_len(modulus);
	public_exp_len = bn_len(public_exp);
	len = public_exp_len + get_byte_number(public_exp_len) + 1;
	len += (bn_get_bit_number(public_exp) % 8 == 0);
	len += modulus_len + get_byte_number(modulus_len)
						+ (modulus_len >= 0x80) + 1;
	len += (bn_get_bit_number(modulus) % 8 == 0);
	len += get_byte_number(len) + (len >= 0x80) + 1;
	len++;
	len += get_byte_number(len) + (len >= 0x80) + 1;
	len += 13;
	len += 2;
	len += get_byte_number(len) + (len >= 0x80) + 1;
	return (len);
}

static void	update_public_data(uint32_t *remain_len, uint32_t *len,
								unsigned char *public_data)
{
	if ((*remain_len)-- >= 0x80)
		public_data[(*len)++] = 0x80 + get_byte_number((*remain_len)--);
}

void		fill_pem_public_data(unsigned char *public_data,
										uint32_t public_data_len,
										t_bn *modulus,
										t_bn *public_exp)
{
	uint32_t		len;
	uint32_t		remain_len;

	len = 0;
	remain_len = public_data_len;
	public_data[len++] = 0x30;
	remain_len--;
	update_public_data(&remain_len, &len, public_data);
	set_len_to_data(remain_len, public_data, &len);
	public_data[len++] = 0x30;
	public_data[len++] = 0x0D;
	ft_memcpy(public_data + len, g_magic, 13);
	len += 13;
	public_data[len++] = 0x03;
	remain_len = public_data_len - len;
	update_public_data(&remain_len, &len, public_data);
	set_len_to_data(remain_len, public_data, &len);
	public_data[len++] = 0x0;
	public_data[len++] = 0x30;
	remain_len = public_data_len - len;
	update_public_data(&remain_len, &len, public_data);
	set_len_to_data(remain_len, public_data, &len);
	set_bn_to_data(modulus, public_data, &len);
	set_bn_to_data(public_exp, public_data, &len);
}

static int	parse_decoded_public_data(t_rsa_data *rsa_data,
										unsigned char *data,
										uint32_t length)
{
	uint32_t		len;
	unsigned char	*ptr;

	ptr = data;
	len = 0;
	if (*ptr++ != 0x30)
		return (1);
	len = get_number_len(&ptr);
	if (*ptr++ != 0x30)
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	ptr += *ptr;
	if (*ptr++ != 0x00)
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	if (*ptr++ != 0x03)
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	len = get_number_len(&ptr);
	if (*ptr++ != 0x00)
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	if (*ptr++ != 0x30)
		return (ft_dprintf(STDERR_FILENO, "ft_ssl: asn1 decoding failed\n"));
	len = get_number_len(&ptr);
	return (retrieve_modulus_and_public(rsa_data, &ptr, data, length));
}

int			retrieve_data_from_public_key(t_rsa_data *rsa_data,
												char *public_data)
{
	uint32_t		public_data_decoded_len;
	uint32_t		public_data_len;
	unsigned char	*public_data_decoded;
	int				ret;

	public_data_len = ft_strlen(public_data);
	public_data_decoded_len = 0;
	if (public_data_len % 4)
		return (1);
	public_data_decoded = base64_decode_data(&public_data_decoded_len,
												ft_strdup(public_data),
												public_data_len);
	if (public_data_decoded == NULL)
		return (1);
	ret = parse_decoded_public_data(rsa_data,
									public_data_decoded,
									public_data_decoded_len);
	ft_memdel((void**)&public_data_decoded);
	return (ret);
}
