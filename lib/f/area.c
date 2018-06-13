#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/area.h"

#define T HeFArea

struct T {
    int n;
    real *area, *darea;

    real a0, K;
};

int he_f_area_ini(real a0, real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_nt(he);

    MALLOC(n, &q->darea);
    MALLOC(n, &q->area);

    q->n = n;
    q->a0 = a0;
    q->K = K;
    
    *pq = q;
    return HE_OK;
}

int he_f_area_fin(T *q) {
    FREE(q->area); FREE(q->darea); FREE(q);
    return HE_OK;
}

int he_f_area_a(T *q, /**/ real  **pa) {
    *pa = q->area;
    return HE_OK;
}

int he_f_area_da(T *q, /**/ real  **pa) {
    *pa = q->darea;
    return HE_OK;
}

static void get_ijk(int t, He *he, /**/ int *pi, int *pj, int *pk) {
    int h, n, nn, i, j, k;
    h = he_hdg_tri(he, t);
    n = he_nxt(he, h);
    nn = he_nxt(he, n);
    i = he_ver(he, h); j = he_ver(he, n); k = he_ver(he, nn);
    *pi = i; *pj = j; *pk = k;
}
static void get(int t, He *he,
                real *x, real *y, real *z, /**/
                real a[3], real b[3], real c[3]) {
    int i, j, k;
    get_ijk(t, he, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}

static void compute_area(real a0, He *he, real *x, real *y, real *z, /**/ real *area, real *darea) {
    real one, delta, a[3], b[3], c[3];    
    int n, t;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        area[t]  = one   = tri_area(a, b, c);
        darea[t] = delta = a0 - one;
    }
}

void compute_force(real K, real *darea, He *he, real *x, real *y, real *z, /**/ real *fx, real *fy, real *fz) {
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;
    n = he_nt(he);    
    for (t = 0; t < n; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);

        dtri_area(a, b, c, /**/ da, db, dc);
        coeff = 2*K*darea[t];
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);        
    }
}
    
int he_f_area_force(T *q, He *he,
                      real *x, real *y, real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *area, *darea, a0, K;
    n = q->n;
    area = q->area;
    darea = q->darea;
    a0 = q->a0;
    K  = q->K;
    
    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_area(a0, he, x, y, z, /**/ area, darea);
    compute_force(K, darea, he, x, y, z, /**/ fx, fy, fz);
    
    return HE_OK;
}
