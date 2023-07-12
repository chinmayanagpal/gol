#include "structs.h"
#include <stdlib.h>
// remove this later:
#include <stdio.h>
#include <limits.h>

bool coordcmp(struct coord a, struct coord b)
{
	return (a.row == b.row && a.col == b.col);
}

bool spmat_at(struct spmat* mat, struct coord pos)
{
	while (mat) {
		if (coordcmp(mat->val, pos)) {
			return true;
		}
		mat = mat->next;
	}
	return false;
}

void toggle_spmat(struct spmat** head, struct coord pos)
{
	struct spmat* this = *head;
	struct spmat* prev = NULL;
	while (this) {
		if (coordcmp(this->val, pos)) {
			if (prev)
				prev->next = this->next;
			else 
				*head = this->next;
			free(this);
			return;

		}
		prev = this;
		this = this->next;
	}
	this = malloc(sizeof(struct spmat));
	this->val = pos;
	this->next = *head;
	*head = this;
}

// clears the spmat at *head
void clear_spmat(struct spmat** head)
{
	while(*head)
	{
		pop_spmat(head);
	}
}

struct coord pop_spmat(struct spmat** head)
{
	struct coord val = (*head)->val;
	struct spmat* next = (*head)->next;
	free(*head);
	*head = next;
	return val;
}

// clones src and returns it
struct spmat* clone_spmat(struct spmat* src)
{
	if (src == NULL)
		return NULL;
	struct spmat* dst = malloc(sizeof(struct spmat));
	struct spmat* this = dst;
	while(src) {
		this->val = src->val;
		if (src->next) {
			this->next = malloc(sizeof(struct spmat));
		} else {
			this->next = NULL;
		}
		src = src->next;
		this = this->next;
	}
	return dst;
}

struct spmat* expanded_spmat(struct spmat* src)
{
	if (src == NULL)
		return NULL;
	struct spmat* dst = NULL;
	struct spmat* this = dst;
	while(src) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				struct coord pos = src->val;
				pos.row += i;
				pos.col += j;
				spmat_set(&dst, pos, true);
			}
		}
		src = src->next;
	}
	return dst;
}

// returns coord containing maximum row and column in mat
struct coord max_spmat(struct spmat *mat)
{
	struct coord max = {INT_MIN, INT_MIN};
	struct spmat *this = mat;
	while (this) {
		if ((this->val).row > max.row)
			max.row = (this->val).row;
		if ((this->val).col > max.col)
			max.col = (this->val).col;
		this = this->next;
	}
	return max;
}

// returns coord containing minimum row and column in mat
struct coord min_spmat(struct spmat *mat)
{
	struct coord min = {INT_MAX, INT_MAX};
	struct spmat *this = mat;
	while (this) {
		if ((this->val).row < min.row)
			min.row = (this->val).row;
		if ((this->val).col < min.col)
			min.col = (this->val).col;
		this = this->next;
	}
	return min;
}

void spmat_set(struct spmat** head, struct coord pos, bool a)
{
	struct spmat* this = *head;
	struct spmat* prev = NULL;

	while (this) {
		if (coordcmp(this->val, pos)) {
			if (!a) {
				if (prev)
					prev->next = this->next;
				else 
					*head = this->next;
				free(this);
			}
			return;
		}
		prev = this;
		this = this->next;
	}
	if (a) {
		this = malloc(sizeof(struct spmat));
		this->val = pos;
		this->next = *head;
		*head = this;
	}
}

void print_spmat(struct spmat* head)
{
	int n = 0;
	while(head) {
		printf("%d: (%d %d)\n", n, head->val.row, head->val.col);
		head = head->next;
		n++;
	}
}
