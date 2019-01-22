/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bjanik <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 11:18:58 by bjanik            #+#    #+#             */
/*   Updated: 2019/01/17 12:14:17 by bjanik           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int		set_options(t_ssl_command *command, char **argv, int *index)
{
	int	ret;

	if ((ret = parse_opt(command, argv, index)))
		return (ret);
	if (command->msg->str || command->msg->fd > -1)
	{
		command->hash_func(command->msg, command->opts);
		reset_msg(command->msg);
	}
	return (0);
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

static void		reset_des(t_ssl_command *cmd)
{
	ft_strdel(&cmd->des->password);
	ft_strdel((char**)&cmd->des->salt);
	ft_memdel((void**)&cmd->des->base64);
	ft_memset(cmd->des->hex_keys, '0', MAX_KEY_LEN * 4);
	ft_memdel((void**)&cmd->des);
}

static int		data_encryption_standard(char **argv, t_ssl_command *cmd)
{
	int 			ret;

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

static int		ft_ssl_routine(char **argv)
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
	else
		ret = base64_core(argv, command->base64);
	return (ret);
}

int				interactive_mode(char **argv)
{
	t_lexer	lexer;
	char	*input;
	t_list	*lst;
	char	**av;

	init_lexer(&lexer);
	while (42)
	{
		write(STDOUT_FILENO, "ft_ssl> ", 8);
		if (get_next_line(STDIN_FILENO, &input) < 0)
			ft_error_msg("ft_ssl: Reading user input failed");
		if (!input)
			break ;
		if (lexer_input(&lexer, input))
			continue ;
		if (!(lst = ft_lstnew(argv[0], ft_strlen(argv[0]) + 1)))
			ft_error_msg("Malloc failed");
		lst->next = lexer.tokens[0];
		lexer.tokens[0] = lst;
		if (!(av = lst_to_tab(lexer.tokens[0], lexer.count + 1)))
			ft_error_msg("Malloc failed");
		ft_ssl_routine(av);
		ft_strdel(&input);
		reset_lexer(&lexer);
		ft_free_string_tab(&av);
	}
	ft_strdel(&lexer.current_token);
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
