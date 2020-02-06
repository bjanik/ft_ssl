/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/23 12:36:58 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "gmp.h"

static int		hash_files(char **argv, t_ssl_command *command)
{
	int	i;
	int	ret;
	int	rett;

	i = 0;
	rett = 0;
	while (argv[i])
	{
		if ((ret = init_msg(command->msg, NULL, argv[i++])) != 0)
		{
			rett = ret;
			continue ;
		}
		command->hash_func(command->msg, command->opts);
		close(command->msg->fd);
	}
	return (rett);
}

static int		hash_algo(char **argv, t_ssl_command *command)
{
	int	i;

	i = 2;
	while (argv[i] && ft_strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		if (set_options(command, argv, &i))
			return (1);
		i++;
	}
	(argv[i] && !ft_strcmp(END_OF_OPT, argv[i])) ? i++ : 0;
	if (!argv[i] && !(command->opts & (OPT_S | OPT_P)))
	{
		init_msg(command->msg, NULL, NULL);
		command->hash_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	i = hash_files(argv + i, command);
	ft_memdel((void**)&command->msg);
	return (i);
}

static int		data_encryption_standard(char **argv, t_ssl_command *cmd)
{
	int	ret;

	ret = 0;
	if (des_opts(argv, cmd->des))
		ret = 1;
	else
	{
		if (!(cmd->des->opts & DES_OPT_K) && !(cmd->des->opts & DES_OPT_D))
			generate_keys_vector(cmd->des);
		if (cmd->des->opts & DES_OPT_CAP_P)
			display_skv(cmd->des);
		else if (cmd->des->opts & DES_OPT_D && !ret)
			des_decrypt_message(cmd->des);
		else if (!ret)
			des_encrypt_message(cmd->des);
	}
	reset_des(cmd);
	return (ret);
}


static int 		genrsa_command(char **argv, t_ssl_command *cmd)
{
	int 	ret = 0;

	(void)argv;
	if (genrsa_opts(argv, cmd->genrsa))
		ret = 1;
	mpz_t 	p, q, n, phi, p_1, q_1, eps, gcd, t, s;


	mpz_init2(p, cmd->genrsa->numbits);
	mpz_init2(q, cmd->genrsa->numbits);
	mpz_init2(p_1, cmd->genrsa->numbits);
	mpz_init2(q_1, cmd->genrsa->numbits);
	mpz_init2(phi, cmd->genrsa->numbits * 2);
	mpz_init2(n, cmd->genrsa->numbits * 2);
	mpz_inits(gcd, t, s, NULL);

	mpz_init2(eps, 16);
	mpz_add_ui(eps, eps, 0x10001);

	if (set_random_mpz_size(p, cmd->genrsa->numbits / 2) < 0 ||	set_random_mpz_size(q, cmd->genrsa->numbits / 2) < 0)
	{
		mpz_clears(p, q, n, phi, p_1, q_1, NULL);
		return (1);
	}	

	if (mpz_probab_prime_p(n, 10) == 0)
		mpz_nextprime(q, q);
	if (mpz_probab_prime_p(n, 10) == 0)
		mpz_nextprime(p, p);
	mpz_mul(n, p, q);
	mpz_sub_ui(p_1, p, 1);
	mpz_sub_ui(q_1, q, 1);
	mpz_mul(phi, p_1, q_1);
	mpz_gcdext(gcd, s, t, phi, eps); // s * phi + eps * t = gcd ; t is the private key
	// If gcd is not equal to 1, it means phi and eps and not coprime so we have to start all over again
	if (gcd->_mp_size == 1 && gcd->_mp_d[0] == 1)
	{
		printf("phi and eps are coprime!\nPrivate key is in hexa:\n");
		display_mpz(t);
	}
		

	return (ret);
}

int				ft_ssl_routine(char **argv)
{
	t_ssl_command	*command;
	int				ret;

	if (!argv[1])
		return (1);
	if (!(command = get_ssl_command(argv[1])))
		return (commands_usage(argv[1]));
	if (command->hash_func)
		ret = hash_algo(argv, command);
	else if (command->des)
		ret = data_encryption_standard(argv, command);
	else if (command->base64)
		ret = base64_core(argv, command->base64);
	else
		ret = genrsa_command(argv, command);
	return (ret);
}

int				main(int argc, char **argv)
{
	int			ret;

	if (argc == 1)
		ret = interactive_mode(argv);
	else
		ret = ft_ssl_routine(argv);
	return (ret);
}
