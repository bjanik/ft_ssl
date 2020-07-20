#include "bn.h"
#include "gmp.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

# define ITERATION 100
# define SIZE_MAX 590

int     cmp_bn_mpz(t_bn *n, mpz_t m)
{
    if (ABS(n->size) != ABS(m->_mp_size))
    {
        // printf("size mpz = %d\n", m->_mp_size);
        printf("ERROR SIZE\n");
        return 1;
    }
    for (int64_t i = 0; i < ABS(n->size); i++)
    {
        if (n->num[i] != m->_mp_d[i])
        {
            printf("ERROR VAL\n");
            return 1;
        }
    }
    return 0;
}

void   dup_bn(mpz_t m, t_bn *n)
{
    mpz_init2(m, SIZE(n) * 64);
    for (int64_t i = 0; i < n->alloc; i++)
        m->_mp_d[i] = n->num[i];
    m->_mp_size = n->size;
}

void    set_randomly_mpz_bn(t_bn *n, mpz_t m, int64_t size)
{
    unsigned char   buffer[4096];
    int             fd = open("/dev/random", O_RDONLY);
    int             to_read = 0;
    uint32_t        bits;

    if (size % 8)
        to_read++;
    to_read += size / 8;
    read(fd, buffer, to_read);
    memcpy((uint8_t*)n->num, buffer, to_read);
    n->size = size / 64;
    if (size % 64 && n->num[n->size])
        n->size++;
    bits = bn_get_bit_number(n);
    if (bits > size)
        bn_shift_right(n, bits - size);
    else if (bits < size)
        bn_shift_left(n, size - bits);
    for (int64_t i = 0; i < SIZE(n); i++)
        m->_mp_d[i] = n->num[i];
    m->_mp_size = n->size;
    close(fd);
}

void    display_mpz(mpz_t n)
{
    for (int64_t i = ABS(n->_mp_size) - 1; i > -1; i--)
    {
        if (i == ABS(n->_mp_size) - 1)
            printf("%lX", n->_mp_d[i]);
        else
            printf("%016lX", n->_mp_d[i]);
    }
    printf("\n");
}

void    test_mul(uint64_t size)
{
    t_bn    *x, *y, *z;
    mpz_t   a, b, c;

    x = bn_init_size(size);
    y = bn_init_size(size);
    z = bn_init_size(size);

    mpz_init2(a, size);
    mpz_init2(b, size);
    mpz_init2(c, size);

    set_randomly_mpz_bn(y, a, size);
    set_randomly_mpz_bn(x, b, size);
    mpz_mul(c, a, b);
    bn_mul(z, y, x);
    cmp_bn_mpz(z, c);
    bn_clears(3, &x, &y, &z);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
}

void    test_add(int64_t size)
{
    t_bn    *x, *y, *z;
    mpz_t   a, b, c;

    x = bn_init_size(size);
    y = bn_init_size(size);
    z = bn_init_size(size);
    if (!x || !y || !z)
    {
        bn_clears(3, &x, &y, &z);
        return ;
    }
    mpz_init2(a, size);
    mpz_init2(b, size);
    mpz_init2(c, size);

    set_randomly_mpz_bn(x, a, size / 2);
    set_randomly_mpz_bn(y, b, size);
    bn_add(z, x, y);
    mpz_add(c, a, b);
    cmp_bn_mpz(z, c);

    // set_randomly_mpz_bn(x, a, size);
    // set_randomly_mpz_bn(y, b, size);
    // bn_add(z, x, y);
    // mpz_add(c, a, b);
    // cmp_bn_mpz(z, c);
    bn_clears(3, &x, &y, &z);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
}

void    test_pow(uint64_t val, uint64_t size)
{
    t_bn        *x, *y, *z;
    mpz_t       a, b, c;
    uint64_t    blocks = size / 64;

    x = bn_init_size(size);
    y = bn_init_size(64);
    z = bn_init_size(size);

    mpz_init2(a, size);
    mpz_init2(b, size);
    mpz_init2(c, size);

    for (uint64_t i = 0; i < size / 64; i++)
    {
        x->num[i] = a->_mp_d[i] = val;
        b->_mp_d[i] = val;
    }
    x->size = a->_mp_size = blocks;
    b->_mp_size = blocks;
    y->size = 1;
    y->num[0] = val;
    mpz_pow_ui(a, b, val);
    printf("MPZ : \n");
    display_mpz(a);
    bn_pow(z, x, y);
    printf("BN : \n");
    display_bn(z);
}

void    test_shift_left(int size, uint64_t shift)
{
    t_bn        *x = bn_init_size(size);
    mpz_t       a;

    mpz_init2(a, size);
    set_randomly_mpz_bn(x, a, size);

    display_bn(x);
    bn_shift_left(x, shift);
    display_bn(x);
    bn_clear(&x);
}

void    test_shift_right(int size, uint64_t shift)
{
    t_bn        *x = bn_init_size(size);
    mpz_t       a;

    mpz_init2(a, size);
    set_randomly_mpz_bn(x, a, size);


    display_bn(x);
    bn_shift_right(x, shift);
    display_bn(x);
    bn_clear(&x);
}


void    test_div(uint64_t size)
{
    t_bn        *q, *r, *n, *d;
    mpz_t       mq, mr, mn, md;

    q = bn_init_size(size);
    r = bn_init_size(size);
    n = bn_init_size(size * 2);
    d = bn_init_size(size);
    mpz_init2(mq, size);
    mpz_init2(mr, size);
    mpz_init2(mn, size * 2);
    mpz_init2(md, size);

    set_randomly_mpz_bn(n, mn, size * 2);
    set_randomly_mpz_bn(d, md, size);
    if (bn_cmp_ui(d, 0) == 0)
        return ;
    bn_div(q, r, n, d);
    mpz_tdiv_qr(mq, mr, mn, md);
    // display_bn(q);
    // display_mpz(mq);
    cmp_bn_mpz(q, mq);
    cmp_bn_mpz(r, mr);
    // display_bn(r);
    // display_mpz(mr);
}


void    test_sub_ui(uint64_t val, uint64_t size)
{
    t_bn        *y;
    mpz_t       a;

    y = bn_init_size(size);
    mpz_init2(a, size);

    set_randomly_mpz_bn(y, a, size);

    bn_sub_ui(y, y, val);
    mpz_sub_ui(a, a, val);
    cmp_bn_mpz(y, a);
    // display_mpz(a);
    // display_bn(y);
    mpz_clear(a);
    bn_clear(&y);
}

void    test_sub(int64_t size)
{
    t_bn        *x, *a, *b;
    mpz_t       n, p, q;

    x = bn_init_size(size * 2);
    a = bn_init_size(size * 2);
    b = bn_init_size(size);

    mpz_init2(n, size * 2);
    mpz_init2(p, size * 2);
    mpz_init2(q, size); 

    // printf("%lld\n", size * 2);
    set_randomly_mpz_bn(a, p, size * 2);
    set_randomly_mpz_bn(b, q, size);

    // display_stats(a);
    // display_stats(b);
    bn_sub(x, a, b);
    mpz_sub(n, p, q);
    bn_clears(3, &x, &a, &b);
    mpz_clear(n);
    mpz_clear(p);
    mpz_clear(q);
}

void    test_mod(int64_t size)
{
    t_bn        *r, *n, *d;
    mpz_t       a, b, c;

    r = bn_init_size(size);
    n = bn_init_size(size * 2);
    d = bn_init_size(size);

    mpz_init2(a, size);
    mpz_init2(b, size * 2);
    mpz_init2(c, size);

    set_randomly_mpz_bn(n, b, size * 2);
    set_randomly_mpz_bn(d, c, size);

    if (bn_cmp_ui(d, 0) == 0)
        return ;
    bn_mod(r, n, d);
    // display_mpz(c);
    mpz_mod(a, b, c);
    cmp_bn_mpz(r, a);
    bn_clears(3, &r, &n, &d);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
}

void    test_pow_mod(uint64_t size)
{
    t_bn        *res, *base, *exp, *mod;
    mpz_t       r, b, e, m;

    res = bn_init_size(size);
    base = bn_init_size(size);
    exp = bn_init_size(size);
    mod = bn_init_size(size * 2);
    mpz_init2(r, size);
    mpz_init2(b, size);
    mpz_init2(e, size);
    mpz_init2(m, size * 2);

    set_randomly_mpz_bn(base, b, size);
    set_randomly_mpz_bn(exp, e, size);
    set_randomly_mpz_bn(mod, m, size * 2);

    bn_mod_pow(res, base, exp, mod);
    mpz_powm(r, b, e, m);
    cmp_bn_mpz(res, r);
    bn_clears(4, &res, &base, &exp, &mod);
    mpz_clear(r);
    mpz_clear(b);
    mpz_clear(e);
    mpz_clear(m);
}

int test_isprime(uint64_t size)
{
    t_bn            *n = NULL;
    mpz_t           m;
    int             ret = 0;
    int             ret2 = 0;

    mpz_init2(m, size);
    n = bn_init_size(size);
    set_randomly_mpz_bn(n, m, size);
    ret = miller_rabin(n, 5, NO_DISPLAY);
    ret2 = mpz_probab_prime_p(m, 10);
    bn_clear(&n);
    mpz_clear(m);
    printf("%d %d\n",ret, ret2);
    return ret;
}

int test_gcd(int64_t size)
{
    t_bn *gcd, *x, *y;
    mpz_t g, b, c;

    gcd = bn_init_size(size);
    x = bn_init_size(size);
    y = bn_init_size(size);

    mpz_init2(g, size);
    mpz_init2(b, size);
    mpz_init2(c, size);

    set_randomly_mpz_bn(x, b, size);
    set_randomly_mpz_bn(y, c, size);

    bn_gcd(gcd, x, y);
    mpz_gcd(g, b, c);
    cmp_bn_mpz(gcd, g);
    bn_clears(3, &gcd, &x, &y);
    mpz_clear(g);
    mpz_clear(b);
    mpz_clear(c);
    return 0;

}

// int     test_gcdext(uint64_t size)
// {
//     // Calculate extented euclid 
//     // int64_t gcd, s, t, a, b;
//     t_bn    *gcd, *s, *t, *a, *b;
//     mpz_t   gcdm, sm, tm, am, bm;
//     t_bn    *first = bn_init_size(size * 100);
//     t_bn    *second = bn_init_size(size * 100);
//     t_bn    *res = bn_init_size(size);

//     mpz_init2(gcdm, size);
//     mpz_init2(sm, size);
//     mpz_init2(tm, size);
//     mpz_init2(am, size);
//     mpz_init2(bm, size);

//     gcd = bn_init_size(size);
//     a = bn_init_size(size);
//     b = bn_init_size(size);
//     s = bn_init_size(size);
//     t = bn_init_size(size);

//     // set_randomly_mpz_bn(a, am, size);
//     // set_randomly_mpz_bn(b, bm, size);
//     while (miller_rabin(a, 150, 0) && miller_rabin(b, 150, 0))
//     {
//         set_randomly_mpz_bn(a, am, size);
//         set_randomly_mpz_bn(b, bm, size);
//     }
//     bn_gcdext(a, b, s, t, gcd);
//     display_bn(gcd);
//     bn_mul(first, a, s);
//     bn_mul(second, b, t);
//     // display_bn(first);
//     // display_bn(second);

//     bn_sub(res, first, second);
//     display_bn(res);

//     // mpz_gcdext(gcdm, sm, tm, am, bm);
//     // cmp_bn_mpz(t, tm);
//     // display_bn(t);
//     // display_mpz(tm);
//     printf("----------------------------------------\n");
//     return 0;
// }

int main()
{
    // for (int i = 0; i < ITERATION; i++)
    // {
    //     test_gcd(64);
    //     test_gcd(128);
    //     test_gcd(256);
    //     test_gcd(258);
    //     test_gcd(328);
    //     test_gcd(440);
    //     test_gcd(1024);
    // }
    // printf("GCD PASSED\n");
    // for (int j = 0; j < ITERATION; j++)
    // {
    //     for (int i = 8; i < SIZE_MAX; i++)
    //         test_div(i);
    // }
    // printf("GCD EXT PASSED\n");
    // // for (int i = 0; i < ITERATION; i++)
    // // {
    // //  test_isprime(64);
    // //  test_isprime(128);
    // //  test_isprime(256);
    // //  test_isprime(384);
    // //  test_isprime(512);
    // //  test_isprime(1024);
    // //  test_isprime(768);
    // //  test_isprime(2048);
    // // }
    // // printf("PRIME PASSED\n");
    // for (int i = 0; i < ITERATION; i++)
    // {
    //     test_sub_ui(0x6DDAA0001, 520);
    //     test_sub_ui(0x100998001, 258);
    //     test_sub_ui(0x100088801, 130);
    //     test_sub_ui(0x10001, 128);
    //     test_sub_ui(0xFFDDAEE12878C, 1024);
    // }
    // printf("SUB UI PASSED\n");
    // for (int i = 0; i < ITERATION; i++)
    // {
    //     test_gcdext(32);
    // }
    // for (int i = 0 ; i < ITERATION; i++)
    // {
    //     for (int j = 2; j < SIZE_MAX; j++)
    //         test_sub(128);
    // }
    // printf("SUB PASSED\n");
    // for (int i = 0 ; i < ITERATION; i++)
    // {
    //     for (int j = 2; j < SIZE_MAX; j++)
    //         test_add(j);
    // }
    // printf("ADD PASSED\n");
    // for (int i = 0 ; i < ITERATION; i++)
    // {
    //     for (int j = 2; j < SIZE_MAX; j++)
    //         test_mul(j);
    // }
    // printf("MUL PASSED\n");
    // for (int i = 0 ; i < ITERATION; i++)
    // {
    //     for (int j = 2; j < SIZE_MAX; j++)
    //         test_mod(j);
    // }
    // printf("MOD PASSED\n");

    // while (1);
    for (int i = 0 ; i < ITERATION; i++)
    {
        test_pow_mod(64);
        test_pow_mod(128);
        test_pow_mod(258);
        test_pow_mod(300);
        test_pow_mod(512);
        test_pow_mod(1028);
        // test_pow_mod(2048);
        // test_pow_mod(4096);
        // test_pow_mod(384);
        // test_pow_mod(256);
        // test_pow_mod(1024);
        // test_pow_mod(768);
        printf("LOOP PASSED\n");
    }
    printf("POW MOD PASSED\n");
    return 0;
}