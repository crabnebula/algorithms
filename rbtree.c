#include "rbtree.h"

struct rb_node *rb_minimum(struct rb_node *node, struct rb_node *sentinel)
{
	while (node->left != sentinel)
		node = node->left;

	return node;
}

struct rb_node *rb_maximum(struct rb_node *node, struct rb_node *sentinel)
{
	while (node->right != sentinel)
		node = node->right;

	return node;
}

struct rb_node *rb_successor(struct rb_node *node, struct rb_node *sentinel)
{
	if (node->right != sentinel) {
		node = node->right;
		while (node->left != sentinel)
			node = node->left;
		return node;
	}

	while (node->parent != sentinel && node == node->parent->right)
		node = node->parent;

	return node;
}

struct rb_node *rb_predecessor(struct rb_node *node, struct rb_node *sentinel)
{
	if (node->left != sentinel) {
		node = node->left;
		while (node->right != sentinel)
			node = node->right;
		return node;
	}

	while (node->parent != sentinel && node == node->parent->left)
		node = node->parent;

	return node;
}

static void rb_left_rotate(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *y = node->right;
	struct rb_node *sentinel = tree->sentinel;

	node->right = y->left;
	if (y->left != sentinel)
		y->left->parent = node;
	
	y->parent = node->parent;
	if (node->parent == sentinel)
		tree->root = y;
	else if (node == node->parent->left)
		node->parent->left = y;
	else
		node->parent->right = y;

	y->left = node;
	node->parent = y;
}

static void rb_right_rotate(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *y = node->left;
	struct rb_node *sentinel = tree->sentinel;

	node->left = y->right;
	if (y->right != sentinel)
		y->right->parent = node;

	y->parent = node->parent;
	if (node->parent == sentinel)
		tree->root = y;
	else if (node == node->parent->left)
		node->parent->left = y;
	else
		node->parent->right = y;

	y->right = node;
	node->parent = y;
}

struct rb_node *rb_insert(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node **pp = &tree->root;
	struct rb_node *sentinel = tree->sentinel;
	struct rb_node *parent = sentinel;
	struct rb_node *uncle;
	struct rb_node *new = node;

	while (*pp != sentinel) {
		parent = *pp;
		if (node->key < parent->key)
			pp = &parent->left;
		else if (node->key > parent->key)
			pp = &parent->right;
		else
			return parent;
	}

	*pp = node;
	node->parent = parent;
	node->left = sentinel;
	node->right = sentinel;
	node->color = RB_RED;

	while (node->parent->color == RB_RED) {
		if (node->parent == node->parent->parent->left) {
			uncle = node->parent->parent->right;
			if (uncle->color == RB_RED) {
				node->parent->color = RB_BLACK;
				uncle->color = RB_BLACK;
				node->parent->parent->color = RB_RED;
				node = node->parent->parent;
			} else {
				if (node == node->parent->right) {
					node = node->parent;
					rb_left_rotate(tree, node);
				}
				node->parent->color = RB_BLACK;
				node->parent->parent->color = RB_RED;
				rb_right_rotate(tree, node->parent->parent);
			}
		} else {
			uncle = node->parent->parent->left;
			if (uncle->color == RB_RED) {
				node->parent->color = RB_BLACK;
				uncle->color = RB_BLACK;
				node->parent->parent->color = RB_RED;
				node = node->parent->parent;
			} else {
				if (node == node->parent->left) {
					node = node->parent;
					rb_right_rotate(tree, node);
				}
				node->parent->color = RB_BLACK;
				node->parent->parent->color = RB_RED;
				rb_left_rotate(tree, node->parent->parent);
			}
		}
	}
	tree->root->color = RB_BLACK;

	return new;
}

static inline void rb_transplant(struct rb_tree *tree, struct rb_node *u, 
		struct rb_node *v)
{
	if (u->parent == tree->sentinel)
		tree->root = u;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	v->parent = u->parent;
}

void rb_delete(struct rb_tree *tree, struct rb_node *node)
{
	struct rb_node *subst = node;
	struct rb_node *x, *w;
	struct rb_node *sentinel = tree->sentinel;
	int color;

	subst = node;
	color = subst->color;
	if (node->left == sentinel) {
		x = node->right;
		x->parent = node->parent;
		if (node->parent == sentinel)
			tree->root = x;
		else if (node == node->parent->left)
			node->parent->left = x;
		else
			node->parent->right =x;
	} else if (node->right == sentinel) {
		x = node->left;
		x->parent = node->parent;
		if (node->parent == sentinel)
			tree->root = x;
		else if (node == node->parent->left)
			node->parent->left = x;
		else
			node->parent->right = x;
	} else {
		subst = rb_minimum(node->right, sentinel);
		color = subst->color;
		x = subst->right;
		if (subst->parent == node)
			x->parent = subst;
		else {
			x->parent = subst->parent;
			subst->parent->left = x;
			subst->right = node->right;
			subst->right->parent = subst;
		}
		subst->parent = node->parent;
		if (node->parent == sentinel)
			tree->root = subst;
		else if (node == node->parent->left)
			node->parent->left = subst;
		else
			node->parent->right = subst;
		subst->left = node->left;
		subst->left->parent = subst;
		subst->color = node->color;
	}

	if (color != RB_BLACK)
		return;

	while (x != tree->root && x->color == RB_BLACK) {
		if (x == x->parent->left) {
			w = x->parent->right;
			if (w->color == RB_RED) {
				w->color = RB_BLACK;
				x->parent->color = RB_RED;
				rb_left_rotate(tree, x->parent);
				w = x->parent->right;
			}
			if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
				w->color = RB_RED;
				x = x->parent;
			} else {
				if (w->right->color == RB_BLACK) {
					w->color = RB_RED;
					w->left->color = RB_BLACK;
					rb_right_rotate(tree, w);
					w = x->parent->right;
				}
				w->color = x->parent->color;
				x->parent->color = RB_BLACK;
				w->right->color = RB_BLACK;
				rb_left_rotate(tree, x->parent);
				x = tree->root;
			}
		} else {
			w = x->parent->left;
			if (w->color == RB_RED) {
				w->color = RB_BLACK;
				x->parent->color = RB_RED;
				rb_right_rotate(tree, x->parent);
				w = x->parent->left;
			}
			if (w->left->color == RB_BLACK && w->right->color == RB_BLACK) {
				w->color = RB_RED;
				x = x->parent;
			} else {
				if (w->left->color == RB_BLACK) {
					w->color = RB_RED;
					w->right->color = RB_BLACK;
					rb_left_rotate(tree, w);
					w = x->parent->left;
				}
				w->color = x->parent->color;
				x->parent->color = RB_BLACK;
				w->left->color = RB_BLACK;
				rb_right_rotate(tree, x->parent);
				x = tree->root;
			}
		}
	}
	x->color = RB_BLACK;
}
