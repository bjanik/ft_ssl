#include "bn.h"
#include <assert.h>

#define IS_ODD(x) ((x) & 1)
#define IS_EVEN(x) (((x) & 1) == 0)
#define SWAP(x, y) (x ^= y, y ^= x, x ^= y)

void    bn_gcd(t_bn *gcd, t_bn *x, t_bn *y)
{
    bn_copy(gcd, y);
    while (bn_cmp_ui(x, 0) > 0)
    {
        bn_copy(gcd, x);
        bn_mod(x, y, x);
        bn_copy(y, gcd);
    }
}

void    bn_gcdext(t_bn *a, t_bn *b, t_bn *s, t_bn *t, t_bn *gcd)
{
    t_bn    *t1 = bn_clone(b);
    t_bn    *t2 = bn_clone(a);
    t_bn    *t3 = bn_clone(b);
    int     k;

    // printf("START BN_GCDEXT------------------------------------------------------\n");
    if (bn_cmp(a, b) < 0)
        bn_swap(a, b);
    // printf("AFTER FIRST CMP------------------------------------------------------\n");
    for (k = 0; IS_EVEN(a->num[0]) && IS_EVEN(b->num[0]); ++k)
    {
        bn_shift_right(a, 1);
        bn_shift_right(b, 1);
    }
    bn_set_ui(s, 1);
    bn_set_zero(t);
    bn_copy(gcd, a);
    bn_sub_ui(t2, t2, 1);
    // printf("START DO WHILE-----------------------------------------------------------\n");
    do {
        do {
            if (IS_EVEN(gcd->num[0]))
            {
                if (IS_ODD(s->num[0]) || IS_ODD(t->num[0]))
                {
                    bn_add(s, s, b);
                    bn_add(t, t, a);
                }
                bn_shift_right(s, 1);
                bn_shift_right(t, 1);
                bn_shift_right(gcd, 1);
                // printf("gcd = ");
                // display_bn(gcd);
            }
            if (IS_EVEN(t3->num[0]) || bn_cmp(gcd, t3) < 0)
            {
                // printf("t3 = ");
                // display_bn(t3);
                bn_swap(s, t1);
                bn_swap(t, t2);
                bn_swap(gcd, t3);
            }
        } while (IS_EVEN(gcd->num[0]));
        // printf("End do while\n");
        while (bn_cmp(s, t1) < 0 || bn_cmp(t, t2) < 0)
        {
            bn_add(s, s, b);
            bn_add(t, t, a);
        }
        assert(bn_cmp(s, t1) > 0 || bn_cmp(s, t1) == 0);
        assert(bn_cmp(t, t2) > 0 || bn_cmp(t, t2) == 0);
        assert(bn_cmp(gcd, t3) == 0 || bn_cmp(gcd, t3) > 0);
        bn_sub(s, s, t1);
        bn_sub(t, t, t2);
        // printf("gcd = ");
        // display_bn(gcd);
        // printf("t3 = ");
        // display_bn(t3);
        bn_sub(gcd, gcd, t3);
        // printf("gcd2 = ");
        // display_bn(gcd);
    } while (bn_cmp_ui(t3, 0) > 0);
    while (bn_cmp(s, b) >= 0 && bn_cmp(t, a) >= 0)
    {
        bn_sub(s, s, b);
        bn_sub(t, t, a);
    }
    bn_shift_left(s, k);
    bn_shift_left(t, k);
    bn_shift_left(gcd, k);
    bn_clears(3, &t1, &t2, &t3);
}

void    ext_euclid(int64_t *u, int64_t *v, int64_t *u1, int64_t *u2, int64_t *u3)
{
    int64_t k, t1, t2, t3;

    if (*u < *v)
        SWAP(*u, *v);
    for (k = 0; IS_EVEN(*u) && IS_EVEN(*v); ++k)
    {
        *u >>= 1;
        *v >>= 1;
    }
    *u1 = 1 ; *u2 = 0; *u3 = *u; t1 = *v; t2 = *u - 1; t3 = *v;
    do {
        do {
            if (IS_EVEN(*u3)) { 
                if (IS_ODD(*u1) || IS_ODD(*u2)) {
                    *u1 += *v; *u2 += *u;
                }
                *u1 >>= 1; *u2 >>= 1; *u3 >>= 1; 
            }
            if (IS_EVEN(t3) || *u3 < t3){
                SWAP(*u1, t1);
                SWAP(*u2, t2);
                SWAP(*u3, t3);
            }
        } while (IS_EVEN(*u3));
        while (*u1 < t1 || *u2 < t2) {
            *u1 += *v; *u2 += *u;
        }
        *u1 -= t1; *u2 -= t2; *u3 -= t3;
    } while (t3 > 0);
    while (*u1 >= *v && *u2 >= *u)  {
        *u1 -= *v; *u2 -= *u;
    }
    *u1 <<= k; *u2 <<= k; *u3 <<= k;
}