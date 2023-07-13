#include <stdio.h>
#include "files.h"
#include <stdlib.h>
#include <string.h>

spmat* read_life_file(FILE* fp)
{
	spmat* mat = NULL;
	char str[256];
	fgets (str, 256, fp);
	while (fgets (str, 256, fp)!=NULL) {
		coord pos;
		pos.x = atoi(strtok(str, " "));
		pos.y = atoi(strtok(NULL, " "));
		spmat_set(&mat, pos, true);
	}
	return mat;
}

void write_life_file(FILE* fp, spmat* mat)
{
	fprintf(fp, "#Life 1.06\n" );
	while(mat)
	{
		fprintf(fp, "%d %d\n", mat->val.x, mat->val.y);
		mat = mat->next;
	}
}
