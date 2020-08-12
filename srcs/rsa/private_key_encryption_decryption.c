/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_key_encryption_decryption.c                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/11 15:05:39 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/11 15:05:41 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

static unsigned char	*free_data(unsigned char *decrypted_data,
									unsigned char *data)
{
	ft_dprintf(STDERR_FILENO, "ft_ssl: Bad decrypt\n");
	ft_memdel((void**)&decrypted_data);
	ft_memdel((void**)&data);
	return (NULL);
}

unsigned char			*private_key_decryption(t_des *des,
												unsigned char *data,
												uint32_t *data_len,
												char *args[])
{
	unsigned char	*decrypted_data;
	unsigned char	*decrypted_data_no_pad;

	if (key_from_passphrase(des, 1, args[0], args[1]))
		return (NULL);
	swap_keys(des->keys[0]);
	swap_keys(des->keys[1]);
	swap_keys(des->keys[2]);
	decrypted_data = des_decrypt_data(des, data, *data_len);
	if (decrypted_data[*data_len - 1] > 8)
		return (free_data(decrypted_data, data));
	if (ft_memcmp(decrypted_data + *data_len - decrypted_data[*data_len - 1],
					g_padding_patterns[decrypted_data[*data_len - 1] - 1],
					decrypted_data[*data_len - 1]))
		return (NULL);
	*data_len = *data_len - decrypted_data[*data_len - 1];
	if ((decrypted_data_no_pad = (unsigned char*)ft_malloc(*data_len)) == NULL)
		return (NULL);
	ft_memcpy(decrypted_data_no_pad, decrypted_data, *data_len);
	ft_memdel((void**)&decrypted_data);
	ft_memdel((void**)&data);
	return (decrypted_data_no_pad);
}

unsigned char			*private_key_encryption(t_des *des,
												unsigned char *data,
												uint32_t *data_len,
												char *args[])
{
	unsigned char	*des_encrypted_data;

	des_encrypted_data = NULL;
	if (key_from_passphrase(des, 0, args[0], args[1]) == 0)
		des_encrypted_data = des_encrypt_data(des, data, data_len);
	ft_memdel((void**)&data);
	return (des_encrypted_data);
}
