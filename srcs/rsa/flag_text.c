#include "bn.h"
#include "ft_ssl.h"

static void    display_bn_flag_text(t_bn *n, int fd, char *id)
{
    int64_t     pos;
    uint32_t    limb_len, printed;
    uint8_t     val;

    if (SIZE(n) == 0)
        return ;
    pos = SIZE(n) - 1;
    printed = 0;
    ft_putendl_fd(id, fd);
    ft_putstr_fd("    ", fd);
    while (pos > -1)
    {
        limb_len = get_byte_number(n->num[pos]);
        while (limb_len)
        {
            val = (n->num[pos] >> (limb_len - 1) * 8) & 0xFF;
            ft_printf("%02hhx", val);
            if (pos != 0 || limb_len != 1)
                ft_putchar(':');
            printed++;
            if (printed % 15 == 0)
                ft_putstr_fd("\n    ", fd);
            limb_len--;
        }
        pos--;
    }
    ft_putchar('\n');
}

void    flag_text(t_rsa *rsa)
{
    display_bn_flag_text(rsa->rsa_data.modulus, rsa->fd[OUT], "modulus:");
    ft_putendl_fd("publicExponent: 65537 (0x10001)", rsa->fd[OUT]);
    if ((rsa->opts & RSA_PUBIN) == 0)
    {
        display_bn_flag_text(rsa->rsa_data.private_exp, rsa->fd[OUT], "privateExponent:");
        display_bn_flag_text(rsa->rsa_data.prime1, rsa->fd[OUT], "prime1:");
        display_bn_flag_text(rsa->rsa_data.prime2, rsa->fd[OUT], "prime2");
        display_bn_flag_text(rsa->rsa_data.exponent1, rsa->fd[OUT], "exponent1:");
        display_bn_flag_text(rsa->rsa_data.exponent2, rsa->fd[OUT], "exponent2:");
        display_bn_flag_text(rsa->rsa_data.coef, rsa->fd[OUT], "coefficient:");
    }
}