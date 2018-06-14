#include <stdio.h>
#include <math.h>

#include <real.h>

#include <he/err.h>
#include <he/vec.h>
#include <he/punto.h>

#include <he/x.h>
#include <alg/x.h>
#include <alg/min.h>

real Energy(const real *x, const real *y, const real *z) {
    real a, v, e;
    a = f_area_energy(x, y, z);
    v = f_volume_energy(x, y, z);
    e = f_harmonic_energy(x, y, z);
    return a + v + e;
}

static void zero(int n, real *a) {
    int i;
    for (i = 0; i < n; i++) a[i] = 0;
}
void Force(const real *x, const real *y, const real *z, /**/
           real *fx, real *fy, real *fz) {
    zero(NV, fx); zero(NV, fy); zero(NV, fz);
    f_area_force(x, y, z, /**/ fx, fy, fz);
    f_volume_force(x, y, z, /**/ fx, fy, fz);
    f_harmonic_force(x, y, z, /**/ fx, fy, fz);
}

static void main0() {
    int i;
    real *queue[] = {XX, YY, ZZ, NULL};
    punto_fwrite(NV, queue, stdout);
    printf("\n");
    for (i = 0; i < 1000000; i++) {
        min_position(/**/ XX, YY, ZZ);
        if (i % 100 == 0) {
            punto_fwrite(NV, queue, stdout);
            printf("\n");
            MSG("%.16e", min_energy());

            off_write(XX, YY, ZZ, "q.off");
        }
        min_iterate();
    }
}

int main() {
    real v0, Kv, a0, Ka, e0, Ke;
    ini("/dev/stdin");

    a0 = 0.006809515625;  Ka =  5.0;
    v0 = 1.5606;          Kv =  5.0;
    e0 = 0.1;             Ke =  0.0;

    f_volume_ini(v0, Kv);
    f_area_ini(a0, Ka);
    f_harmonic_ini(e0, Ke);

    min_ini(STEEPEST_DESCENT);

    main0();

    min_fin();
    f_harmonic_fin();
    f_volume_fin();
    f_area_fin();
    fin();
    return 0;
}
