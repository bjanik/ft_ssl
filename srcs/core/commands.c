/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/10 17:21:51 by bjanik            #+#    #+#             */
/*   Updated: 2020/08/10 17:21:52 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"

int	genrsa_command(char **argv, t_ssl_command *cmd)
{
	t_genrsa	*genrsa;
	int			ret;

	(void)cmd;
	ret = 0;
	genrsa = genrsa_init();
	if (genrsa_opts(argv, genrsa))
		ret = 1;
	else if (genrsa_command_run(&genrsa->rsa_data, genrsa))
		ret = 1;
	if (ret == 0)
		pem(&genrsa->rsa_data, genrsa->fd[OUT]);
	genrsa_clear(genrsa);
	return (0);
}

int	rsa_command(char **argv, t_ssl_command *cmd)
{
	t_rsa	*rsa;
	int		ret;

	(void)cmd;
	ret = 0;
	rsa = rsa_init();
	if (rsa_opts(argv, rsa))
		ret = 1;
	else if (rsa_command_run(rsa))
		ret = 1;
	rsa_clear(rsa);
	return (ret);
}

int	rsautl_command(char **argv, t_ssl_command *cmd)
{
	t_rsautl	*rsautl;
	int			ret;

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

int	digest_command(char **argv, t_ssl_command *cmd)
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

int	des_command(char **argv, t_ssl_command *cmd)
{
	t_des	*des;
	int		ret;

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
