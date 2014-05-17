#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rbtree.h"

void rb_preorder(struct rb_node *root, struct rb_node *sentinel)
{
	if (root != sentinel) {
		rb_preorder(root->left, sentinel);		
		printf("node %p parent %p left %p right %p color %d key %d\n", 
				root, root->parent, root->left, root->right, root->color, root->key);
		rb_preorder(root->right, sentinel);
	}
}

int main(int argc, char *argv[])
{
	struct rb_node sentinel;
	struct rb_tree rbtree;
	struct rb_node node[1000];
	int i, j;

	rb_init(&rbtree, &sentinel);
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
		rb_insert(&rbtree, &node[i]);
	}
	for (i = 0; i < 10; i++)
		rb_delete(&rbtree, &node[i]);

	exit(0);
}
