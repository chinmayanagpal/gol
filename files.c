#include <stdio.h>
#include "files.h"
#include <stdlib.h>
#include <string.h>

struct spmat* read_life_file(FILE* fp)
{
	struct spmat* mat = NULL;
	char str[256];
	fgets (str, 256, fp);
	while (fgets (str, 256, fp)!=NULL) {
		struct coord pos;
		pos.row = atoi(strtok(str, " "));
		pos.col = atoi(strtok(NULL, " "));
		spmat_set(&mat, pos, true);
	}
	return mat;
}

void write_life_file(FILE* fp, struct spmat* mat)
{
	fprintf(fp, "#Life 1.06\n" );
	while(mat)
	{
		fprintf(fp, "%d %d\n", mat->val.row, mat->val.col);
		mat = mat->next;
	}
}
