#include "ft_ssl.h"
#include "bn.h"

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


int        bn_modinv(t_bn *u, t_bn *v, t_bn *modinv)
{
    t_bn    *u1, *gcd, *v1, *v3, *t1, *t3, *q;
    int     iter, ret;

    ret = 0;
    u1 = bn_init_size(SIZE(v) * 64);
    gcd = bn_clone(u);
    v1 = bn_init_size(SIZE(v) * 64);
    v3 = bn_clone(v);
    t1 = bn_init_size(SIZE(v) * 64);
    t3 = bn_init_size(SIZE(v) * 64);
    q = bn_init_size(SIZE(v) * 64);
    if (!u1 || !gcd || !v1 || !v3 || !t1 || !t3 || !q)
    {
        bn_clears(7, &u1, &gcd, &v1, &v3, &t1, &t3, &q);
        return (1);
    }
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
        ret = 1;
    else if (iter < 0)
        bn_sub(modinv, v, u1);
    else
        bn_copy(modinv, u1);
    bn_clears(7, &u1, &gcd, &v1, &v3, &t1, &t3, &q);
    return (ret);
}