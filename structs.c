#include "structs.h"
#include <stdlib.h>
// remove this later:
#include <stdio.h>

bool coordcmp(struct coord a, struct coord b)
{
	return (a.row == b.row && a.col == b.col);
}

void toggle_sparsematrix(struct sparsematrix** mat, struct coord pos)
{
	struct sparsematrix* this = *mat;
	struct sparsematrix* prev = NULL;
	while (this) {
		if (coordcmp(this->val, pos)) {
			if (prev)
				prev->next = this->next;
			else 
				*mat = this->next;
			free(this);
			return;

		}
		prev = this;
		this = this->next;
	}
	this = malloc(sizeof(struct sparsematrix));
	this->val = pos;
	this->next = *mat;
	*mat = this;
}

void clear_sparsematrix(struct sparsematrix **head)
{
	struct sparsematrix* this;
	while(*head)
	{
		this = *head;
		*head = (*head)->next;
		free(this);
	}
}
struct coord max_sparsematrix(struct sparsematrix *mat)
{
	struct coord max = {0, 0};
	struct sparsematrix *this = mat;
	while (this) {
		if ((this->val).row > max.row)
			max.row = (this->val).row;
		if ((this->val).col > max.col)
			max.col = (this->val).col;
	}
	return max;
}

struct coord min_sparsematrix(struct sparsematrix *mat)
{
	struct coord max = {0, 0};
	struct sparsematrix *this = mat;
	while (this) {
		if ((this->val).row < max.row)
			max.row = (this->val).row;
		if ((this->val).col < max.col)
			max.col = (this->val).col;
	}
	return max;
}
