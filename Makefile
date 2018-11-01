# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bjanik <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/01 17:15:11 by bjanik            #+#    #+#              #
#    Updated: 2018/11/01 17:23:42 by bjanik           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl

CC = gcc

FLAGS = -Wall -Wextra -Werror

SRCS_DIR = srcs/

SRCS =  ft_ssl.c
		main.c
		msg.c
		opt.c
		output.c
		padding.c
		update.c
		usage.c
		utils.c
		md5/md5.c
		md5/md5_transform.c
		sha1/sha1.c
		sha1/sha1_transform.c
		sha256/sha256.c
		sha256/sha256_transform.c



