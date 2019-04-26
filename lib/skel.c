#include <stdio.h>
#include <tgmath.h>

#include "real.h"
#include "co/arc.h"
#include "co/err.h"
#include "co/memory.h"
#include "co/skel.h"

#define PI (3.141592653589793)

#define T Skel
struct T
{
	int nv, Close;
};

int
skel_close_ini(int nv, T **pq)
{
	T *q;

	if (nv <= 0)
		ERR(CO_INDEX, "nv=%d <= 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 1;
	*pq = q;
	return CO_OK;
}

int
skel_open_ini(int nv, T **pq)
{
	T *q;
	if (nv <= 0)
		ERR(CO_INDEX, "nv=%d <= 0", nv);
	MALLOC(1, &q);
	q->nv = nv;
	q->Close = 0;
	*pq = q;
	return CO_OK;
}

int
skel_circle_ini(int nv, /**/ real *length, real **px, real **py, T **pq)
{
	real *x, *y;
	real lo, hi, d, p;
	int i;
	
	if (nv <= 1)
		ERR(CO_INDEX, "nv=%d <= 1", nv);
	MALLOC(nv, &x);
	MALLOC(nv, &y);

	lo = 0;
	hi = 2*PI;
	d = (lo - hi)/nv;
	for (i = 0; i < nv; i++) {
		p = lo + d*i;
		x[i] = cos(p);
		y[i] = sin(p);
	}

	skel_close_ini(nv, pq);
	*length = 2*PI;
	*px = x;
	*py = y;
	return CO_OK;
}

int
skel_fin(T *q)
{
	FREE(q);
	return CO_OK;
}

int
skel_xy_fin(real *x, real *y, T *q)
{
	FREE(x);
	FREE(y);
	FREE(q);
	return CO_OK;
}

int
skel_write(T *q, const real *x, const real *y, FILE *f)
{
	int nv, Close, i, npolylines = 1;
	real z = 0;

	Close = q->Close;
	nv = q->nv;
	if (fputs("SKEL\n", f) == EOF)
		ERR(CO_IO, "fail to write");
	fprintf(f, "%d %d\n", nv, npolylines);
	for (i = 0; i < nv; i++)
		fprintf(f, FMT " " FMT " " FMT "\n", x[i], y[i], z);
	fprintf(f, "%d", Close ? nv + 1 : nv);
	for (i = 0; i < nv; i++)
		fprintf(f, " %d", i);
	fprintf(f, Close ? " 0\n" : "\n");
	return CO_OK;
}

int
skel_nv(T *q)
{
	return q->nv;
}

int
skel_ne(T *q)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	return Close ? nv + 1 : nv;
}

int
skel_bnd(T *q, int i)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	if (i < 0) ERR(CO_INDEX, "i=%d < 0", i);
	if (i >= nv) ERR(CO_INDEX, "i=%d >= nv=%d", i, nv);
	if (Close)
		return 0;
	else
		return i == 0 || i == nv - 1;
}

int
skel_edg_ij(T *q, int e, int *i, int *j)
{
	int Close, nv;

	Close = q->Close;
	nv = q->nv;
	if (e < 0) ERR(CO_INDEX, "e=%d < 0", e);
	if (Close && e >= nv) ERR(CO_INDEX, "e=%d >= nv=%d", e, nv);
	if (!Close &&e >= nv - 1) ERR(CO_INDEX, "e=%d >= nv=%d", e, nv);
	if (Close) {
		*i = e;
		*j = e + 1;
		if (*j == nv)
			*j = 0;
	}
	else {
		*i = e;
		*j = e + 1;
	}
	return CO_OK;
}