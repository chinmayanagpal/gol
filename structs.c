#include "structs.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

bool coordcmp(coord a, coord b)
{
	return (a.x == b.x && a.y == b.y);
}

bool spmat_at(spmat* mat, coord pos)
{
	while (mat) {
		if (coordcmp(mat->val, pos)) {
			return true;
		}
		mat = mat->next;
	}
	return false;
}

void toggle_spmat(spmat** head, coord pos)
{
	spmat* it = *head;
	spmat* prev = NULL;
	while (it) {
		if (coordcmp(it->val, pos)) {
			if (prev)
				prev->next = it->next;
			else 
				*head = it->next;
			free(it);
			return;

		}
		prev = it;
		it = it->next;
	}
	it = (spmat*) malloc(sizeof(spmat));
	it->val = pos;
	it->next = *head;
	*head = it;
}

// clears the spmat at *head
void clear_spmat(spmat** head)
{
	while(*head)
	{
		pop_spmat(head);
	}
}

coord pop_spmat(spmat** head)
{
	coord val = (*head)->val;
	spmat* next = (*head)->next;
	free(*head);
	*head = next;
	return val;
}

// clones src and returns it
spmat* clone_spmat(spmat* src)
{
	if (src == NULL)
		return NULL;
	spmat* dst = (spmat*) malloc(sizeof(spmat));
	spmat* it = dst;
	while(src) {
		it->val = src->val;
		if (src->next) {
			it->next = (spmat*) malloc(sizeof(spmat));
		} else {
			it->next = NULL;
		}
		src = src->next;
		it = it->next;
	}
	return dst;
}

spmat* expanded_spmat(spmat* src)
{
	if (src == NULL)
		return NULL;
	spmat* dst = NULL;
	spmat* it = dst;
	while(src) {
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				coord pos = src->val;
				pos.x += i;
				pos.y += j;
				spmat_set(&dst, pos, true);
			}
		}
		src = src->next;
	}
	return dst;
}

// returns coord containing maximum x and yumn in mat
coord max_spmat(spmat *mat)
{
	coord max = {INT_MIN, INT_MIN};
	spmat *it = mat;
	while (it) {
		if ((it->val).x > max.x)
			max.x = (it->val).x;
		if ((it->val).y > max.y)
			max.y = (it->val).y;
		it = it->next;
	}
	return max;
}

// returns coord containing minimum x and yumn in mat
coord min_spmat(spmat *mat)
{
	coord min = {INT_MAX, INT_MAX};
	spmat *it = mat;
	while (it) {
		if ((it->val).x < min.x)
			min.x = (it->val).x;
		if ((it->val).y < min.y)
			min.y = (it->val).y;
		it = it->next;
	}
	return min;
}

void spmat_set(spmat** head, coord pos, bool a)
{
	spmat* it = *head;
	spmat* prev = NULL;

	while (it) {
		if (coordcmp(it->val, pos)) {
			if (!a) {
				if (prev)
					prev->next = it->next;
				else 
					*head = it->next;
				free(it);
			}
			return;
		}
		prev = it;
		it = it->next;
	}
	if (a) {
		it = (spmat*) malloc(sizeof(spmat));
		it->val = pos;
		it->next = *head;
		*head = it;
	}
}

void print_spmat(spmat* head)
{
	int n = 0;
	while(head) {
		printf("%d: (%d %d)\n", n, head->val.x, head->val.y);
		head = head->next;
		n++;
	}
}
