#ifndef BN_H
# define BN_H
# include <stdint.h>


# define ABS(x) ((x > 0) ? x : -x)
# define SIZE(n) (ABS(n->size))
# define BIGGEST_BN(x, y) ((x->size >= y->size) ? x : y)
# define INC_SIZE(x) (((x)->size < 0) ? (x)->size-- : (x)->size++)
# define DEC_SIZE(x) (((x)->size < 0) ? (x)->size++ : (x)->size--)
# define MIN(x, y) ((x < y) ? x : y)
# define MAX(x, y) ((x > y) ? x : y)
# define IS_ODD(x) ((x) & 1)
# define IS_EVEN(x) (((x) & 1) == 0)

# define NO_DISPLAY 0
# define DISPLAY 1

typedef struct 	big_num
{
	uint64_t	*num; // Array of long long int representing the big num
	int64_t		size; // Biggest index of num array not set to 0. For instance, if num == ULLONG_MAX + 1, size is 2. If num == 1, size is 1. If num == 0, size is 0
	int64_t		alloc; // Capacity of array num. Alloc must always be >= size
}				t_bn;

void			bn_set_zero(t_bn *n);
// int				bn_concat(t_bn *n, t_bn *low, t_bn *high);
t_bn 			*bn_init_size(uint64_t size);
t_bn			*bn_init(void);
int 			bn_set_random(t_bn *n, int64_t size);
void			display_bn(t_bn *n);
int				bn_realloc(t_bn *n);
int 			bn_realloc_size(t_bn *n, int64_t new_alloc);
int 			bn_realloc_size_zero(t_bn *n, int64_t new_alloc);

t_bn 			*bn_clone(t_bn *a); // Returns a freshly allocated copy of bn a
int				bn_copy(t_bn *a, t_bn *b); // Copy value of b into a. Reallocate a if necessary
void			bn_set_ui(t_bn *n, uint64_t ui);
int 			bn_add_ui(t_bn *n, uint64_t ui);
int				bn_add(t_bn *res, t_bn *op1, t_bn *op2);
void			bn_sub(t_bn *res, t_bn *op1, t_bn *op2);
void			bn_sub_ui(t_bn *res, t_bn *a, uint64_t ui);
void 			bn_shift_right(t_bn *n, int64_t shift);
void 			bn_shift_left(t_bn *n, int64_t shift);
void			bn_mul(t_bn *res, t_bn *m, t_bn *q);
void			bn_clear(t_bn **n);
void			bn_clears(int num, ...);
int 			bn_cmp(t_bn *a, t_bn *b);
int 			bn_cmp_ui(t_bn *a, uint64_t ui);
int				bn_pow(t_bn *a, t_bn *b, t_bn *c);
void			bn_div(t_bn *q, t_bn *rem, t_bn *num, t_bn *den);
void			bn_mod(t_bn *r, t_bn *n, t_bn *d);
void			bn_mod_pow(t_bn *res, t_bn *b, t_bn *exp, t_bn *mod);
// void			bn_swap(t_bn *a, t_bn *b);
void			bn_gcd(t_bn *gcd, t_bn *a, t_bn *b);
int 	   		bn_modinv(t_bn *u, t_bn *v, t_bn *modinv);

void			bn_set_bit(t_bn *n, uint64_t pos);


int				get_strongest_bit_pos(t_bn *n);
uint32_t 		bn_len(t_bn *n);
uint32_t 		get_byte_number(uint64_t limb);
uint32_t 		bn_get_bit_number(t_bn *n);
uint64_t 		bn_get_bit(t_bn *n, uint64_t pos);

t_bn    		*get_bn(unsigned char **data, uint32_t len);

void 			power_of_two(t_bn *n, unsigned int pow);



int 			witness(t_bn *a, t_bn *b);
int				miller_rabin(t_bn *n, int s, int display);
int 			pseudo_prime(t_bn *n);
int 			initial_sieve_test(t_bn *n, int display);

void			display_stats(t_bn *n);

// void			ext_euclid(int64_t *u, int64_t *v, int64_t *u1, int64_t *u2, int64_t *u3);



#endif