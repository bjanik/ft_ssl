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

static int		hash_files(char **argv, int opts, t_ssl_command *command)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (argv[i])
		ret = command->hash_func(opts, NULL, argv[i++]);
	return (ret);
}

int		digest_command(char **argv, t_ssl_command *cmd)
{
	int	i;
	int opts;

	i = 2;
	while (argv[i] && ft_strcmp(END_OF_OPT, argv[i]) && argv[i][0] == '-')
	{
		if (set_options(cmd, &opts, argv, &i))
			return (1);
		i++;
	}
	(argv[i] && !ft_strcmp(END_OF_OPT, argv[i])) ? i++ : 0;
	if (!argv[i] && !(opts & (OPT_S | OPT_P)))
		cmd->hash_func(opts, NULL, NULL);
	i = hash_files(argv + i, opts, cmd);
	return (i);
}

int		des_command(char **argv, t_ssl_command *cmd)
{
	t_des 		*des;
	int			ret;

	(void)cmd;
	ret = 0;
	des = init_des(cmd->name, cmd->des_mode);
	if (des_opts(argv, des))
		ret = 1;
	else
	{
		if (!(des->opts & DES_OPT_K) && !(des->opts & DES_OPT_D))
			generate_keys_vector(des);
		if (des->opts & DES_OPT_CAP_P)
			display_skv(des);
		else if (des->opts & DES_OPT_D && !ret)
			des_decrypt_message(des);
		else if (!ret)
			des_encrypt_message(des);
	}
	reset_des(des);
	return (ret);
}

int 		genrsa_command(char **argv, t_ssl_command *cmd)
{
	t_genrsa 	*genrsa;

	(void)cmd;
	genrsa = genrsa_init();
	if (genrsa_opts(argv, genrsa))
		return (1);
	if (genrsa_command_run(&genrsa->rsa_data, genrsa))
		return (1);
	pem(&genrsa->rsa_data, genrsa->fd[OUT]);
	genrsa_clear(genrsa);
	return (0);
}

int 		rsa_command(char **argv, t_ssl_command *cmd)
{
	t_rsa	*rsa;
	int 	ret;

	(void)cmd;
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

int 		rsautl_command(char **argv, t_ssl_command *cmd)
{
	t_rsautl	*rsautl;
	int 		ret;

	(void)cmd;
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
	if (!(command = get_ssl_command(argv)))
		return (commands_usage(argv[1]));
	return (command->func(argv, command));
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