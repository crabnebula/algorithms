#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "krbtree.h"

#ifndef FALSE
#define FALSE	0
#endif

#ifndef TRUE
#define TRUE	1
#endif

struct mytype {
	struct rb_node node;
	int key;
};

struct mytype *my_search(struct rb_root *root, int key)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct mytype *data = container_of(node, struct mytype, node);
		int result = key = data->key;
		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}

	return NULL;
}

int my_insert(struct rb_root *root, struct mytype *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	while (*new) {
		struct mytype *this = container_of(*new, struct mytype, node);	
		int result = data->key - this->key;
		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else
			return FALSE;
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return TRUE;
}

int main(int argc, char *argv[])
{
	struct rb_root mytree = RB_ROOT;
	struct mytype node[1000];
	struct rb_node *ptr;
	int i, j;

	srand(time(0));

	for (i = 0; i < 10; i++) {
		for (;;) {
			node[i].key = rand()%10000;
			for (j = 0; j < i; j++) {
				if (node[i].key == node[j].key)
					break;
			}
			if (j < i)
				continue;
			else
				break;
		}
		my_insert(&mytree, &node[i]);
	}
	for (ptr = rb_first(&mytree); ptr; ptr = rb_next(ptr))
		printf("key=%d\n", rb_entry(ptr, struct mytype, node)->key);
	for (i = 0; i < 10; i++)
		rb_erase(&node[i].node, &mytree);

	exit(0);
}
