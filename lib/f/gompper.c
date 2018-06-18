#include <stdio.h>

#include "real.h"
#include "he/memory.h"
#include "he/err.h"
#include "he/he.h"
#include "he/vec.h"
#include "he/tri.h"
#include "he/dih.h"
#include "he/ddih.h"

#include "he/f/gompper.h"

#define T HeFGompper

#define  nxt(h)     he_nxt(he, (h))
#define  flp(h)     he_flp(he, (h))
#define  ver(h)     he_ver(he, (h))
#define  hdg_ver(v) he_hdg_ver(he, (v))
#define  hdg_edg(e) he_hdg_edg(he, (e))
#define  bnd(h)     he_bnd(he, (h))


struct T {
    int n;
    real *area;
    real *lx, *ly, *lz;

    real *l2, *t;
    real K;
};

int he_f_gompper_ini(real K, He *he, T **pq) {
    T *q;
    int n, nh;
    MALLOC(1, &q);

    nh = he_nh(he);
    MALLOC(nh, &q->l2);
    MALLOC(nh, &q->t);

    n = he_nv(he);
    MALLOC(n, &q->area);
    MALLOC(n, &q->lx);
    MALLOC(n, &q->ly);
    MALLOC(n, &q->lz);
    MALLOC(n, &q->lz);

    q->n = n;
    q->K = K;

    *pq = q;
    return HE_OK;
}

int he_f_gompper_fin(T *q) {
    FREE(q->area);
    FREE(q->lx); FREE(q->ly); FREE(q->lz);
    FREE(q->t); FREE(q->l2);
    FREE(q);
    return HE_OK;
}

int he_f_gompper_area(T *q, /**/ real **pa) {
    *pa = q->area;
    return HE_OK;
}

int he_f_gompper_laplace(T *q, /**/ real **px, real **py, real **pz ) {
    *px = q->lx;
    *py = q->ly;
    *pz = q->lz;
    return HE_OK;
}

enum {BULK, BND};
static int get_ijkl(int e, He *he, /**/ int *pi, int *pj, int *pk, int *pl) {
    int h, n, nn, nnf, i, j, k, l;
    h = he_hdg_edg(he, e);
    if (bnd(h)) return BND;

    h = hdg_edg(e); n = nxt(h); nn = nxt(nxt(h));
    nnf = nxt(nxt(flp(h)));
    j = ver(h); k = ver(n); i = ver(nn); l = ver(nnf);

    *pi = i; *pj = j; *pk = k; *pl = l;
    return BULK;
}

static int get(int e, He *he, const real *x, const real *y, const real *z,
               /**/ real a[3], real b[3], real c[3], real d[3]) {
    int status, i, j, k, l;
    status = get_ijkl(e, he, /**/ &i, &j, &k, &l);
    if (status == BND) return BND;
    vec_get(i, x, y, z, /**/ a);
    vec_get(j, x, y, z, /**/ b);
    vec_get(k, x, y, z, /**/ c);
    vec_get(l, x, y, z, /**/ d);
    return BULK;
}

static void compute_force(real K,
                          He *he, const real *x, const real *y, const real *z, /**/
                          real *fx, real *fy, real *fz) {
    int status, n, t, i, j, k, l;
    real a[3], b[3], c[3], d[3], da[3], db[3], dc[3], dd[3];
    n = he_nv(he);
    for (t = 0; t < n; t++) {
        status = get_ijkl(t, he, /**/ &i, &j, &k, &l);
        if (status == BND) continue;
        vec_get(i, x, y, z, /**/ a);
        vec_get(j, x, y, z, /**/ b);
        vec_get(k, x, y, z, /**/ c);
        vec_get(l, x, y, z, /**/ d);
        ddih_cos(a, b, c, d, /**/ da, db, dc, dd);
        vec_scalar_append(da, 2*K, i, /**/ fx, fy, fz);
        vec_scalar_append(db, 2*K, j, /**/ fx, fy, fz);
        vec_scalar_append(dc, 2*K, k, /**/ fx, fy, fz);
        vec_scalar_append(dd, 2*K, l, /**/ fx, fy, fz);
    }
}

static void get_edg(int i, int j, const real *x, const real *y, const real *z, /**/ real r[3]) {
    real a[3], b[3];
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_minus(a, b, r);
}
static void get3(int i, int j, int k,
                 const real *x, const real *y, const real *z,
                 /**/ real a[3], real b[3], real c[3]) {
    vec_get(i, x, y, z, a);
    vec_get(j, x, y, z, b);
    vec_get(k, x, y, z, c);
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
static void compute_l2(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int h, n;
    int i, j;
    real r[3];
    n = he_nh(he);
    for (h = 0; h < n; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        get_edg(i, j, x, y, z, /**/ r);
        H[h] = vec_dot(r, r);
    }
}

static void compute_t(He *he, const real *x, const real *y, const real *z, /**/ real *H) {
    int nh, h, n, nn;
    int i, j, k;
    real a[3], b[3], c[3], cot;
    nh = he_nh(he);
    zero(nh, H);
    for (h = 0; h < nh; h++) {
        n = nxt(h); nn = nxt(n);
        i = ver(h); j = ver(n); k = ver(nn);
        get3(i, j, k, x, y, z, /**/ a, b, c);
        cot = tri_cot(b, c, a);
        H[h] += cot;
        if (!bnd(h)) H[flp(h)] += cot;
    }
}

static void compute_area(He *he, const real *l2, const real *t, /**/ real *V) {
    int nv, nh, h, i;
    nv = he_nv(he);
    zero(nv, V);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        i = ver(h);
        V[i] += t[h]*l2[h]/8;
    }
}

static void compute_laplace(He *he, const real *V0, const real *t, const real *area, /**/ real *V1) {
    int h, n, nv, nh, i, j;
    nv = he_nv(he);
    zero(nv, V1);
    nh = he_nh(he);
    for (h = 0; h < nh; h++) {
        n = nxt(h);
        i = ver(h); j = ver(n);
        V1[i] += t[h]*(V0[i] - V0[j])/2;
    }
    for (i = 0; i < nv; i++) V1[i] /= area[i];
}

int he_f_gompper_force(T *q, He *he,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz) {
    int n;
    real K;
    n = q->n;
    K  = q->K;
    if (he_nv(he) != n)
        ERR(HE_INDEX, "he_nv(he)=%d != n = %d", he_nv(he), n);
    compute_l2(he, x, y, z, /**/ q->l2);
    compute_t(he, x, y, z, /**/ q->t);
    compute_area(he, q->l2, q->t, /**/ q->area);
    compute_laplace(he, x, q->t, q->area, /**/ q->lx);
    compute_laplace(he, y, q->t, q->area, /**/ q->ly);
    compute_laplace(he, z, q->t, q->area, /**/ q->lz);

    compute_force(K, he, x, y, z, /**/ fx, fy, fz);
    return HE_OK;
}

real he_f_gompper_energy(T *q, He *he,
                      const real *x, const real *y, const real *z) {
    int n;
    real K;
    n = q->n;
    K  = q->K;

    if (he_nv(he) != n)
        ERR(HE_INDEX, "he_nv(he)=%d != n = %d", he_nv(he), n);
    return 2*K;
}
