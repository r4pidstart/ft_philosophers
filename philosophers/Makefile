# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/31 12:48:31 by tjo               #+#    #+#              #
#    Updated: 2022/12/22 05:04:01 by tjo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CFLAGS = -Wall -Wextra -Werror -g3 -ggdb -pthread

SRCS = ft_main.c ft_init.c ft_monitor.c ft_philo.c ft_error.c

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

%.o: %.c
	cc $(CFLAGS) -c $^ -o $@
	
bonus:
	$(MAKE) BONUS=1

clean:
	rm -rf */*.o
	rm -rf *.o
	
fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: bonus all clean fclean re