
CFLAGS := -g -Wall -Werror

t_rbtree : t_rbtree.o rbtree.o
	gcc -o $@ $^

rbtree.o : rbtree.c rbtree.h
t_rbtree.o : t_rbtree.c rbtree.h

clean:
	@rm -rfv rbtree.o t_rbtree.o t_rbtree
