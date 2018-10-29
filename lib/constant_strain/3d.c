#include <stdio.h>

#include "real.h"

#include "he/tri.h"
#include "he/dtri.h"
#include "he/vec.h"
#include "he/err.h"
#include "he/macro.h"
#include "he/constant_strain/2d.h"

int constant_strain_force(void *param,
                          real (*F)(void*, real, real), real (*F1)(void*, real, real), real (*F2)(void*, real, real),
                          const real a0[3], const real b0[3], const real c0[3],
                          const real a[3], const real b[3], const real c[3], /**/
                          real da_tot[3], real db_tot[3], real dc_tot[3]) {
    real da[3], db[3], dc[3];
    real dA[3], dB[3], dC[3];
    real ax, ay, bx, by, cx, cy, vx, vy, ux, uy, wx, wy;
    real dvx, dvy, dux, duy, dwx, dwy;
    real deng, area, I1, I2;

    tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &uy, &wx, &wy);
    ux -= bx; uy -= by; /* displace */
    wx -= cx; wy -= cy;

    constant_strain_2d(param, F1, F2,
                       ax = 0, ay = 0, bx, by, cx, cy,
                       vx = 0, vy = 0, ux, uy, wx, wy,
                       &dvx, &dvy, &dux, &duy, &dwx, &dwy,
                       &I1, &I2, &area);
    deng = F(param, I1, I2);    
    tri_2to3(a, b, c, dvx, dvy, /**/ da);
    tri_2to3(a, b, c, dux, duy, /**/ db);
    tri_2to3(a, b, c, dwx, dwy, /**/ dc);

    dtri_area(a, b, c, /**/ dA, dB, dC);

    vec_linear_combination(area, da, deng, dA, /**/ da_tot);
    vec_linear_combination(area, db, deng, dB, /**/ db_tot);
    vec_linear_combination(area, dc, deng, dC, /**/ dc_tot);    

    return HE_OK;
}

static real Dummy(__UNUSED void *param, __UNUSED real I1, __UNUSED real I2) { return 0; }
int constant_strain_energy(void *param, real (*F)(void*, real, real),
                           const real a0[3], const real b0[3], const real c0[3],
                           const real a[3], const real b[3], const real c[3],
                           real *p_eng, real *p_deng) {
    real ax, ay, bx, by, cx, cy, vx, vy, ux, uy, wx, wy;
    real I1, I2, A, eng, deng;

    tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &uy, &wx, &wy);
    ux -= bx; uy -= by; /* displace */
    wx -= cx; wy -= cy;

    constant_strain_2d(param, Dummy, Dummy,
                       ax = 0, ay = 0, bx, by, cx, cy,
                       vx = 0, vy = 0, ux, uy, wx, wy,
                       NULL, NULL, NULL, NULL, NULL, NULL,
                       &I1, &I2, &A);

    deng = F(param, I1, I2);
    eng = deng * A;

    *p_eng = eng;
    *p_deng = deng;

    return HE_OK;
}

int constant_strain_force_density(void *param, real (*F1)(void*, real, real), real (*F2)(void*, real, real),
                                  const real a0[3], const real b0[3], const real c0[3],
                                  const real a[3], const real b[3], const real c[3], /**/
                                  real da[3], real db[3], real dc[3]) {
    real ax, ay, bx, by, cx, cy, vx, vy, ux, uy, wx, wy;
    real dvx, dvy, dux, duy, dwx, dwy;

    tri_3to2(a0, b0, c0, /**/ &bx, &by, &cx, &cy);
    tri_3to2(a, b, c, /**/ &ux, &uy, &wx, &wy);
    ux -= bx; uy -= by; /* displace */
    wx -= cx; wy -= cy;

    constant_strain_2d(param, F1, F2,
                       ax = 0, ay = 0, bx, by, cx, cy,
                       vx = 0, vy = 0, ux, uy, wx, wy,
                       &dvx, &dvy, &dux, &duy, &dwx, &dwy,
                       NULL, NULL, NULL);

    tri_2to3(a, b, c, dvx, dvy, /**/ da);
    tri_2to3(a, b, c, dux, duy, /**/ db);
    tri_2to3(a, b, c, dwx, dwy, /**/ dc);

    return HE_OK;
}
