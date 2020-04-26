#include "ft_ssl.h"
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

// int         bn_gcdext(t_bn *a, t_bn *b, t_bn *s, t_bn *t, t_bn *gcd)
// {
//     t_bn    *t1 = bn_clone(b);
//     t_bn    *t2 = bn_clone(a);
//     t_bn    *t3 = bn_clone(b);
//     int     k;

//     // display_stats(s);
//     // display_stats(t);
//     // display_stats(gcd);
//     if (t1 == NULL || t2 == NULL || t3 == NULL)
//     {
//         bn_clears(3, &t1, &t2, &t3);
//         return (1);
//     }
//     if (bn_cmp(a, b) < 0)
//         bn_swap(a, b);
//     for (k = 0; IS_EVEN(a->num[0]) && IS_EVEN(b->num[0]); ++k)
//     {
//         bn_shift_right(a, 1);
//         bn_shift_right(b, 1);
//     }
//     bn_set_ui(s, 1);
//     bn_set_zero(t);
//     bn_copy(gcd, a);
//     bn_sub_ui(t2, t2, 1);
//     do {
//         do {
//             if (IS_EVEN(gcd->num[0]))
//             {
//                 if (IS_ODD(s->num[0]) || IS_ODD(t->num[0]))
//                 {
//                     bn_add(s, s, b);
//                     bn_add(t, t, a);
//                 }
//                 bn_shift_right(s, 1);
//                 bn_shift_right(t, 1);
//                 bn_shift_right(gcd, 1);
//             }
//             if (IS_EVEN(t3->num[0]) || bn_cmp(gcd, t3) < 0)
//             {
//                 bn_swap(s, t1);
//                 bn_swap(t, t2);
//                 bn_swap(gcd, t3);
//             }
//         } while (IS_EVEN(gcd->num[0]));
//         while (bn_cmp(s, t1) < 0 || bn_cmp(t, t2) < 0)
//         {
//             bn_add(s, s, b);
//             bn_add(t, t, a);
//         }
//         bn_sub(s, s, t1);
//         bn_sub(t, t, t2);
//         bn_sub(gcd, gcd, t3);
//     } while (bn_cmp_ui(t3, 0) > 0);
//     while (bn_cmp(s, b) >= 0 && bn_cmp(t, a) >= 0)
//     {
//         bn_sub(s, s, b);
//         bn_sub(t, t, a);
//     }
//     bn_shift_left(s, k);
//     bn_shift_left(t, k);
//     bn_shift_left(gcd, k);
//     bn_clears(3, &t1, &t2, &t3);
//     return (0);
// }

// Computes u^(-1) mod v
// v == phi, u is public exp
// Returns d
int        bn_modinv(t_bn *u, t_bn *v, t_bn *modinv)
{
    t_bn    *u1, *gcd, *v1, *v3, *t1, *t3, *q;
    int     iter;

    u1 = bn_init_size(SIZE(v) * 64);
    gcd = bn_clone(u);
    v1 = bn_init_size(SIZE(v) * 64);
    v3 = bn_clone(v);
    t1 = bn_init_size(SIZE(v) * 64);
    t3 = bn_init_size(SIZE(v) * 64);
    q = bn_init_size(SIZE(v) * 64);
    bn_set_ui(u1, 1);    
    iter = 1;
    while (bn_cmp_ui(v3, 0))
    {
        bn_div(q, t3, gcd, v3);
        bn_mul(t1, q, v1);
        bn_add(t1, t1, u1);
        bn_copy(u1, v1);
        bn_copy(v1, t1);
        bn_copy(gcd, v3);
        bn_copy(v3, t3);
        iter = -iter;
    }
    if (bn_cmp_ui(gcd, 1))
        return (1);
    if (iter < 0)
        bn_sub(modinv, v, u1);
    else
        bn_copy(modinv, u1);
    return (0);
}