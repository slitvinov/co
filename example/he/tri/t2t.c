#include <stdio.h>

#include <real.h>
#include <co/err.h>
#include <co/y.h>
#include <co/he.h>

int
main() {
	He      *he;
	real *x, *y, *z;
	int i, j, k, t, nt;
	y_inif(stdin, &he, &x, &y, &z);

	nt = he_nt(he);
	for (t = 0; t < nt; t++) {
	    i = he_hdg_tri(he, t);
	    j = he_nxt(he, i);
	    k = he_nxt(he, j);

	    i = he_flp(he, i);
	    j = he_flp(he, j);
	    k = he_flp(he, k);

	    i = he_tri(he, i);
	    j = he_tri(he, j);
	    k = he_tri(he, k);
	    printf("%d %d %d\n", k, i, j);
	}
	y_fin(he, x, y, z);
	return CO_OK;
}
