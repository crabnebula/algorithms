#ifndef _RB_TREE_H
#define _RB_TREE_H

#define RB_RED		1
#define RB_BLACK	0

struct rb_node {
	struct rb_node *parent;
	struct rb_node *left;
	struct rb_node *right;
	int key, data;
};

struct rb_tree {
	struct rb_node *root;
};

static inline void rb_init(struct rb_tree *tree, struct rb_node *sentinel)
{
	tree->root = sentinel;
	tree->sentinel = sentinel;
	sentinel->color = RB_BLACK;
}

#endif
