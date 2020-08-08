#include "ft_ssl.h"

uint32_t bn_get_bit_number(t_bn *n)
{
	unsigned int 	bits = 64;
	uint64_t		mask = 0x8000000000000000;
	int64_t			size;

	if (SIZE(n) == 0)
		return (0);
	while ((n->num[SIZE(n) - 1] & mask) == 0)
	{
		bits--;
		mask >>= 1;
	}
	size = SIZE(n);
	while (--size)
		bits += 64;
	return (bits);

}

uint32_t get_byte_number(uint64_t limb)
{
	if (limb <= 0xFF)
		return (1);
	if (limb <= 0xFFFF)
		return (2);
	if (limb <= 0xFFFFFF)
		return (3);
	if (limb <= 0xFFFFFFFF)
		return (4);
	if (limb <= 0xFFFFFFFFFF)
		return (5);
	if (limb <= 0xFFFFFFFFFFFF)
		return (6);
	if (limb <= 0xFFFFFFFFFFFFFF)
		return (7);
	return (8);
}

uint32_t bn_len(t_bn *n)
{
    unsigned int    bytes;

    if (SIZE(n) == 0)
    	return (0);
    bytes = (SIZE(n) - 1) * 8;
    return (bytes + get_byte_number(n->num[SIZE(n) - 1]));
}

uint64_t 	bn_get_bit(t_bn *n, uint64_t pos)
{
	uint64_t 	limb;

	limb = pos / 64;
	return (n->num[limb] >> (pos % 64)) & 1;
}

int		get_strongest_bit_pos(t_bn *n)
{
	int 		pos;
	uint64_t	mask;

	mask = (uint64_t)1 << 63;
	pos = 64;
	if (SIZE(n) == 0)
		pos = 0;
	else
	{
		while ((n->num[n->size - 1] & mask) == 0)
		{
			mask >>= 1;
			pos--;
		}
	}
	return (pos);
}