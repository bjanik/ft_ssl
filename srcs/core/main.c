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

#include "bn.h"
#include "ft_ssl.h"
#include "gmp.h"


void    display_mpz(mpz_t n)
{
	if (n->_mp_size == 0)
		printf("0");
	else
	{
	    for (int64_t i = ABS(n->_mp_size) - 1; i > -1; i--)
	    {
	        if (i == ABS(n->_mp_size) - 1)
	            printf("%lX", n->_mp_d[i]);
	        else
	            printf("%016lX", n->_mp_d[i]);
	    }
	}
    printf("\n");
}

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

void	copy_bn_to_mpz(t_bn *n, mpz_t m)
{
	for (int64_t i = 0; i < n->size; i++)
		m->_mp_d[i] = n->num[i];
	m->_mp_size = n->size;
}

static int 		genrsa_command(char **argv, t_ssl_command *cmd)
{
	if (genrsa_opts(argv, cmd->genrsa))
		return (1);
	if (genrsa_command_run(&cmd->genrsa->rsa_data, cmd->genrsa))
		return (1);
	pem(&cmd->genrsa->rsa_data, cmd->genrsa->fd[OUT]);
	return (0);
}

static int 		rsa_command(char **argv, t_ssl_command *cmd)
{
	if (rsa_opts(argv, cmd->rsa))
		return (1);
	if (rsa_command_run(cmd->rsa))
		return (1);
	return (0);
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
		return hash_algo(argv, command);
	if (command->des)
		return data_encryption_standard(argv, command);
	if (command->base64)
		return base64_core(argv, command->base64);
	if (command->genrsa)
		return genrsa_command(argv, command);
	if (command->rsa)
		return rsa_command(argv, command);
	return 0;
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
