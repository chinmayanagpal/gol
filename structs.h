#include <stdbool.h>

struct coord
{
	int row;
	int col;
};

// sparse matrix of booleans
// presence of a coordinate in the linked list denotes a value of 1
// otherwise 0
struct spmat
{
	struct coord val;
	struct spmat *next;
};

/* returns true if coord a and coord b have the same row and col values  */
/* false otherwise */
bool coordcmp(struct coord a, struct coord b);

/* returns the value of mat at pos */
bool spmat_at(struct spmat* mat, struct coord pos);

/* toggles the value of mat at pos */
void toggle_spmat(struct spmat** mat, struct coord pos);

/* deallocates heap memory used by *mat and sets it to NULL */
void clear_spmat(struct spmat **mat);

/* sets *head to a at pos */
void spmat_set(struct spmat** head, struct coord pos, bool a);

void print_spmat(struct spmat* head);

void print_coord(struct coord pos);


struct spmat* clone_spmat(struct spmat* src);

struct spmat* expanded_spmat(struct spmat* src);

struct coord pop_spmat(struct spmat **mat);

/* returns coord containing maximum row and column values of mat */
struct coord max_spmat(struct spmat* mat);

/* returns coord containing minimum row and column values of mat */
struct coord min_spmat(struct spmat* mat);
