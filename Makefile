all: fclean
	make -C philosophers all
fclean:
	make -C philosophers fclean
bonus:
	make -C philosophers bonus
t:
	cc test.c -o test