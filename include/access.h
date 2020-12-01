#ifndef ACCESS_H
#define ACCESS_H

struct tree_node {
	/* serialized */
	int k;            /* one if leaf, otherwise threshold */
	char* attr;       /* attribute string if leaf, otherwise null */
	int label;
    int t;
    int num;
	GPtrArray* children; /* pointers to bswabe_policy_t's, len == 0 for leaves */
};

typedef struct tree_node t_node;

struct satisfied_node {
    t_node * node;
	GPtrArray *children;
	GArray * satified_children;
    int index;
    int satified;
	struct satisfied_node *parent;
};

typedef struct satisfied_node st_node;
#endif