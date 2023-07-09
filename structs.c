#include "structs.h"
#include <stdlib.h>

bool coordcmp(struct coord a, struct coord b)
{
		return (a.row == b.row && a.column == b.column);
}

void toggle_sparsematrix(struct sparsematrix *mat, struct coord pos)
{
		struct sparsematrix *this = mat;
		struct sparsematrix *prev;
		while (this)
		{
				if (coordcmp(this->value, pos))
				{
						prev->next = this->next;
						free(this);
						return;
				}
				prev = this;
				this = this->next;
		}
		this->next = malloc(sizeof(struct sparsematrix));
		(this->next)->value = pos;
		(this->next)->next = NULL;
}

struct coord max_sparsematrix(struct sparsematrix *mat)
{
		struct coord max = {0, 0};
		struct sparsematrix *this = mat;
		while (this)
		{
				if ((this->value).row > max.row)
						max.row = (this->value).row;
				if ((this->value).column > max.column)
						max.column = (this->value).column;
		}
		return max;
}

struct coord min_sparsematrix(struct sparsematrix *mat)
{
		struct coord max = {0, 0};
		struct sparsematrix *this = mat;
		while (this)
		{
				if ((this->value).row < max.row)
						max.row = (this->value).row;
				if ((this->value).column < max.column)
						max.column = (this->value).column;
		}
		return max;
}
