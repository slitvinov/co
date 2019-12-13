#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec.h>
#include <co/tri.h>
#include <co/y.h>

const char *me = "loop/middle";
static void
usg()
{
    fprintf(stderr, "%s OFF > OFF\n", me);
    exit(2);
}

static int t2edg(He *he, int t, int *, int *, int *);
static int t2ver(He *he, int t, int *, int *, int *);

int
main()
{
    He *he;
    int i;
    int j;
    int k;
    int ij;
    int jk;
    int ki;
    int ne;
    int nt;
    int nt0;
    int nv;
    int nv0;
    int t;
    int *tri;
    real *x;
    real *x0;
    real *y;
    real *y0;
    real *z;
    real *z0;
    int ei;
    int ej;
    int ek;
    int i0;

    y_inif(stdin, &he, &x, &y, &z);
    ne = he_ne(he);
    nv = he_nv(he);
    nt = he_nt(he);
    nv0 = nv + ne;
    nt0 = 4*nt;
    MALLOC(nv0, &x0);
    MALLOC(nv0, &y0);
    MALLOC(nv0, &z0);
    MALLOC(3*nt0, &tri);
    

    real a[3];
    real b[3];
    real c[3];
    real ab[3];
    real bc[3];
    real ca[3];
#define ADD(i, j, k) tri[i0++] = (i), tri[i0++] = (j), tri[i0++] = (k)
    for (i0 = t = 0; t < nt; t++)
    {
	t2ver(he, t, &i, &j, &k);
	t2edg(he, t, &ei, &ej, &ek);
	ij = nv + ei;
	jk = nv + ej;
	ki = nv + ek;
	MSG("%d %d %d", ij, jk, ki);
	ADD(i, ij, ki);
	ADD(ij, j, jk);
	ADD(ij, jk, ki);
	ADD(ki, jk, k);

	vec_get3(i, j, k, x, y, z, a, b, c);
	tri_edg_center(a, b, c, ab, bc, ca);
    }
    if (i0 != 3*nt0)
	ER("i0=%d != nt0=%d", i0, nt0);

    FREE(x0);
    FREE(y0);
    FREE(z0);
    FREE(tri);
    y_fin(he, x, y, z);
}

static int
t2edg(He *he, int t, int *i, int *j, int *k)
{
    int h, n, nn;
    
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);

    *i = he_edg(he, h);
    *j = he_edg(he, n);
    *k = he_edg(he, nn);
    
    return CO_OK;
}

static int
t2ver(He *he, int t, int *i, int *j, int *k)
{
    int h, n, nn;
    
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);

    *i = he_ver(he, h);
    *j = he_ver(he, n);
    *k = he_ver(he, nn);
    
    return CO_OK;
}

