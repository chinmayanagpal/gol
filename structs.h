#include <stdbool.h>

struct coord
{
	int row;
	int column;
};

// sparse matrix of booleans
// presence of a coordinate in the linked list denotes a value of 1
// otherwise 0
struct sparsematrix
{
	struct coord value;
	struct sparsematrix *next;
};

// compare two coords
bool coordcmp(struct coord a, struct coord b);

// return coord containing maximum row and column values of mat
struct coord max_sparsematrix(struct sparsematrix *mat);

// return coord containing minimum row and column values of mat
struct coord min_sparsematrix(struct sparsematrix *mat);
