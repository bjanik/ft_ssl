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

static int		des_command(char **argv, t_ssl_command *cmd)
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
	reset_des(cmd->des);
	return (ret);
}

static int 		genrsa_command(char **argv)
{
	t_genrsa 	*genrsa;

	genrsa = genrsa_init();
	if (genrsa_opts(argv, genrsa))
		return (1);
	if (genrsa_command_run(&genrsa->rsa_data, genrsa))
		return (1);
	pem(&genrsa->rsa_data, genrsa->fd[OUT]);
	genrsa_clear(genrsa);
	return (0);
}

static int 		rsa_command(char **argv)
{
	t_rsa	*rsa;
	int 	ret;

	ret = 0;
	rsa = rsa_init();
	if (rsa_opts(argv, rsa))
		ret = 1;
	else if (rsa_command_run(rsa))
		ret = 1;
	if (rsa->in)
		close(rsa->fd[IN]);
	if (rsa->out)
		close(rsa->fd[OUT]);
	rsa_clear(rsa);
	return (ret);
}

static int 		rsautl_command(char **argv)
{
	t_rsautl	*rsautl;
	int 		ret;

	ret = 0;
	rsautl = rsautl_init();
	if (rsautl_opts(argv, rsautl))
		ret = 1;
	else if (rsautl_command_run(rsautl))
		ret = 1;
	rsautl_clear(rsautl);
	return (ret);
}

int				ft_ssl_routine(char **argv)
{
	t_ssl_command	*command;

	if (!argv[1])
		return (1);
	if (!(command = get_ssl_command(argv[1])))
		return (commands_usage(argv[1]));
	if (command->hash_func)
		return (hash_algo(argv, command));
	if (command->des)
		return (des_command(argv, command));
	if (command->base64)
		return (base64_command(argv));
	if (command->genrsa)
		return (genrsa_command(argv));
	if (command->rsa)
		return (rsa_command(argv));
	if (command->rsautl)
		return (rsautl_command(argv));
	return (0);
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