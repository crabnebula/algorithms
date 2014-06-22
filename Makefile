
CFLAGS := -g -Wall -Werror

bins := t_rbtree r_krbtree
objs := rbtree.o t_rbtree.o krbtree.o t_krbtree.o

all : t_rbtree t_krbtree

t_rbtree : t_rbtree.o rbtree.o
	gcc -o $@ $^

t_krbtree : t_krbtree.o krbtree.o
	gcc -o $@ $^

rbtree.o : rbtree.c rbtree.h
t_rbtree.o : t_rbtree.c rbtree.h
krbtree.o : krbtree.c krbtree_augmented.h
t_krbtree.o : t_krbtree.c krbtree.h

clean:
	@rm -rfv $(bins) $(objs)
