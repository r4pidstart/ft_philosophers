# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/31 12:48:31 by tjo               #+#    #+#              #
#    Updated: 2022/11/25 02:04:54 by tjo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Wextra -Werror -g3 -ggdb #-pthread

LIBFT = ft_mylibft/libft.a
LIBFT_SRCS = ft_mylibft
SRCS = ft_main.c ft_utils.c

ifdef BONUS
	NAME = philo_bonus
	FOLDER = bonus/
	SRCS = 
else
	FOLDER = mandatory/
endif

all: $(NAME)

$(NAME): $(addprefix $(FOLDER), $(SRCS:.c=.o)) #$(LIBFT)
	cc $(CFLAGS) \
	-o ./$(NAME) \
	$(addprefix $(FOLDER), $(SRCS)) \
	#-L$(LIBFT_SRCS) -lft

$(LIBFT):
	make -C $(LIBFT_SRCS) all

%.o: %.c
	cc $(CFLAGS) -c $^ -o $@
	
bonus:
	$(MAKE) BONUS=1

clean:
	rm -rf */*.o
	rm -rf *.o
	make clean -C $(LIBFT_SRCS)
	
fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFT_SRCS)

re: fclean all

.PHONY: bonus all clean fclean re