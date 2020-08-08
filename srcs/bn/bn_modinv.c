#include "ft_ssl.h"
#include "bn.h"

static void init_bns(t_bn *bns[], t_bn *u, t_bn *v)
{
    bns[0] = bn_init_size(SIZE(v) * 64);
    bns[1] = bn_clone(u);
    bns[2] = bn_init_size(SIZE(v) * 64);
    bns[3] = bn_clone(v);
    bns[4] = bn_init_size(SIZE(v) * 64);
    bns[5] = bn_init_size(SIZE(v) * 64);
    bns[6] = bn_init_size(SIZE(v) * 64);
    bn_set_ui(bns[0], 1);
}

static void process(t_bn *bns[])
{
    bn_div(bns[6], bns[5], bns[1], bns[3]);
    bn_mul(bns[4], bns[6], bns[2]);
    bn_add(bns[4], bns[4], bns[0]);
    bn_copy(bns[0], bns[2]);
    bn_copy(bns[2], bns[4]);
    bn_copy(bns[1], bns[3]);
    bn_copy(bns[3], bns[5]);
}

int        bn_modinv(t_bn *u, t_bn *v, t_bn *modinv)
{
    t_bn    *bns[7];
    int     iter;
    int     ret;

    ret = 0;
    iter = 1;
    init_bns(bns, u, v);
    while (bn_cmp_ui(bns[3], 0))
    {
        process(bns);
        iter = -iter;
    }
    if (bn_cmp_ui(bns[1], 1))
        ret = 1;
    else if (iter < 0)
        bn_sub(modinv, v, bns[0]);
    else
        bn_copy(modinv, bns[0]);
    bn_clears(7, &bns[0], &bns[1], &bns[2], &bns[3],
              &bns[4], &bns[5], &bns[6]);
    return (ret);
}