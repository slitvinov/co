#include <math.h>
#include <stdio.h>

#include "real.h"
#include "he/vec.h"
#include "he/tri.h"

real tri_area(real a[3], real b[3], real c[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a, u);
    vec_minus(c, a, v);
    vec_cross(u, v,   n);
    return vec_abs(n)/2;
}

/* oriented volume of tetrahedral [0, a, b, c] */
real tri_volume(real a[3], real b[3], real c[3]) {
    real n[3];
    vec_cross(a, b,   n);
    return vec_dot(c, n)/6;
}

void tri_normal(real a[3], real b[3], real c[3], /**/ real e[3]) {
    real u[3], v[3], n[3];
    vec_minus(b, a,   u);
    vec_minus(c, a,   v);
    vec_cross(u, v,   n);
    vec_norm(n,   e);
}

/* bc is an edge */
real tri_dih(real a[3], real b[3], real c[3], real d[3]) {
    real x, y, ang, n[3], k[3], nk[3], e[3];
    tri_normal(b, c, a,   n);
    tri_normal(c, b, d,   k);
    x = vec_dot(n, k);
    vec_cross(n, k,    nk);
    y = vec_abs(nk);
    ang = atan2(y, x);
    vec_minus(c, b, e);
    if (vec_dot(e, nk) < 0)
	ang = - ang;
    return ang;
}
