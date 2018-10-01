#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dtri.h"

#include "he/f/garea.h"

#define T HeFGarea

struct T {
    int n;
    real *area;
    real a0, K;
};

int he_f_garea_ini(real a0, real K, He *he, T **pq) {
    T *q;
    int n;
    MALLOC(1, &q);
    n = he_nt(he);

    MALLOC(n, &q->area);

    q->n = n;
    q->a0 = a0;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_garea_fin(T *q) {
    FREE(q->area); FREE(q);
    return HE_OK;
}

int he_f_garea_a(T *q, /**/ real  **pa) {
    *pa = q->area;
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
                const real *x, const real *y, const real *z, /**/
                real a[3], real b[3], real c[3]) {
    int i, j, k;
    get_ijk(t, he, &i, &j, &k);
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
}
static void compute_area(real a0, He *he, const real *x, const real *y, const real *z, /**/ real *area, real *darea) {
    real one, a[3], b[3], c[3];
    int n, t;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get(t, he, x, y, z, /**/ a, b, c);
        area[t]  = one   = tri_area(a, b, c);
        darea[t] = one - a0;
    }
}

static void compute_force(real K, real a0, real *darea, He *he, const real *x, const real *y, const real *z,
                          /**/ real *fx, real *fy, real *fz) {
    int n, t, i, j, k;
    real a[3], b[3], c[3], da[3], db[3], dc[3], coeff;
    n = he_nt(he);
    for (t = 0; t < n; t++) {
        get_ijk(t, he, /**/ &i, &j, &k);
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);

        dtri_area(a, b, c, /**/ da, db, dc);
        coeff = 2*K*darea[t]/a0;
        vec_scalar_append(da, coeff, i, /**/ fx, fy, fz);
        vec_scalar_append(db, coeff, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, coeff, k, /**/ fx, fy, fz);
    }
}

int he_f_garea_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real *area, *darea, a0, K;
    n = q->n;
    area = q->area;
    a0 = q->a0;
    K  = q->K;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    compute_area(a0, he, x, y, z, /**/ area, darea);
    compute_force(K, a0, darea, he, x, y, z, /**/ fx, fy, fz);

    return HE_OK;
}

static real sum(int n, real *a) {
    int t;
    real s;
    s = 0;
    for (t = 0; t < n; t++)
        s += a[t];
    return s;
}
real he_f_garea_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int n;
    real *area, a0, a, K;
    n = q->n;
    area = q->area;
    a0 = q->a0;
    K  = q->K;

    if (he_nt(he) != n)
        ERR(HE_INDEX, "he_nt(he)=%d != n = %d", he_nt(he), n);

    a = sum(n, area);
    return K/a0 * (a - a0) * (a - a0);
}
