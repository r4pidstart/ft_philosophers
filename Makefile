all: fclean
	make -C philo_bonus all
fclean:
	make -C philo_bonus fclean
bonus:
	make -C philosophers bonus
t:
	cc test.c -o test