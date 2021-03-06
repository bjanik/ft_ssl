/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bn.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 12:11:26 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 12:11:28 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BN_H
# define BN_H
# include <stdint.h>

# define NO_DISPLAY 0
# define DISPLAY 1

typedef struct	s_big_num
{
	uint64_t	*num;
	int64_t		size;
	int64_t		alloc;
}				t_bn;

void			bn_set_zero(t_bn *n);
t_bn			*bn_init_size(uint64_t size);
t_bn			*bn_init(void);
int				bn_set_random(t_bn *n, int64_t size);
int				bn_realloc(t_bn *n);
int				bn_realloc_size(t_bn *n, int64_t new_alloc);
int				bn_realloc_size_zero(t_bn *n, int64_t new_alloc);

t_bn			*bn_clone(t_bn *a);
int				bn_copy(t_bn *a, t_bn *b);
void			bn_set_ui(t_bn *n, uint64_t ui);
int				bn_add_ui(t_bn *n, uint64_t ui);
int				bn_add(t_bn *res, t_bn *op1, t_bn *op2);
void			bn_sub(t_bn *res, t_bn *op1, t_bn *op2);
void			bn_sub_ui(t_bn *res, t_bn *a, uint64_t ui);
void			bn_shift_right(t_bn *n, int64_t shift);
void			bn_shift_left(t_bn *n, int64_t shift);
void			bn_mul(t_bn *res, t_bn *m, t_bn *q);
void			bn_clear(t_bn **n);
void			bn_clears(int num, ...);
int				bn_cmp(t_bn *a, t_bn *b);
int				bn_cmp_ui(t_bn *a, uint64_t ui);
void			bn_div(t_bn *q, t_bn *rem, t_bn *num, t_bn *den);
void			bn_mod(t_bn *r, t_bn *n, t_bn *d);
void			bn_mod_no_alloc(t_bn *r, t_bn *n, t_bn *d);

void			bn_mod_pow(t_bn *res, t_bn *b, t_bn *exp, t_bn *mod);
void			bn_gcd(t_bn *gcd, t_bn *a, t_bn *b);
int				bn_modinv(t_bn *u, t_bn *v, t_bn *modinv);

void			bn_set_bit(t_bn *n, uint64_t pos);

int				get_strongest_bit_pos(t_bn *n);
uint32_t		bn_len(t_bn *n);
uint32_t		get_byte_number(uint64_t limb);
uint32_t		bn_get_bit_number(t_bn *n);
uint64_t		bn_get_bit(t_bn *n, uint64_t pos);

t_bn			*get_bn(unsigned char **data, uint32_t len);

void			power_of_two(t_bn *n, unsigned int pow);

int				witness(t_bn *a, t_bn *b);
int				miller_rabin(t_bn *n, int s, int display);
int				pseudo_prime(t_bn *n);
int				initial_sieve_test(t_bn *n, int display);
#endif
