#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
#include <real.h>
#include <alg/ode.h>
#include <co/array.h>
#include <co/bi.h>
#include <co/equiangulate.h>
#include <co/err.h>
#include <co/f/garea.h>
#include <co/f/juelicher_xin.h>
#include <co/force.h>
#include <co/f/volume.h>
#include <co/f/volume.h>
#include <co/he.h>
#include <co/len.h>
#include <co/macro.h>
#include <co/matrix.h>
#include <co/memory.h>
#include <co/ode/3.h>
#include <co/off.h>
#include <co/punto.h>
#include <co/y.h>

#define FMT_IN CO_REAL_IN
#define FMT_OUT CO_REAL_OUT

static const char *me = "vesicle";
static const real pi = 3.141592653589793115997964;
static const real tol = 0.0001;
static const int iter_max = 100;

static Force *Fo[20] = { NULL };

static HeFVolume *fvolume;
static He *he;
static BI *bi;
static real rho, eta, lambda, gamdot, dt;
static int start, end, freq_out, freq_stat;
static real *fx, *fy, *fz;
static real *Vx, *Vy, *Vz;
static real *ux, *uy, *uz;
static real *wx, *wy, *wz;
static int nv, nt;

char file_out[999];
char file_stat[99] = "stat.dat";
char file_msg[99] = "msg.out";
FILE *fm;

static void
usg(void)
{
    fprintf(stderr, "%s garea A Kga area a Ka volume V Kv\n", me);
    fprintf(stderr, "juelicher_xin Kb C0 Kad DA0D\n");
    fprintf(stderr, "R D rho eta lamda gamdot dt\n");
    fprintf(stderr, "start end freq_out freq_stat\n");
    fprintf(stderr, "< OFF input file \n");
}

static int
num(char **v, /**/ int *p)
{
    if (*v == NULL) {
        usg();
        ER("not enough args");
    }
    if (sscanf(*v, "%d", p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
scl(char **v, /**/ real * p)
{
    if (*v == NULL)
        ER("not enough args");
    if (sscanf(*v, FMT_IN, p) != 1)
        ER("not a number '%s'", *v);
    return CO_OK;
}

static int
fargv(char ***p, He * he)
{
    char *name, **v;
    int i;

    i = 0;
    v = *p;
    while ((name = v[0]) != NULL && force_good(name)) {
        v++;
        force_argv(name, &v, he, &Fo[i++]);
    }

    name = v[0];
    if (name == NULL)
        ER("expecting BI");
    if (!bi_good(name)) {
        MSG("not a bi name: '%s'", name);
        ER("possible names are: %s", bi_list());
    }
    v++;
    bi_argv(name, &v, he, &bi);

    he_f_volume_ini(3.6678, 10000, he, &fvolume);
    MALLOC3(nv, &Vx, &Vy, &Vz);


    scl(v, &rho);
    v++;
    scl(v, &eta);
    v++;
    scl(v, &lambda);
    v++;
    scl(v, &gamdot);
    v++;
    scl(v, &dt);
    v++;
    num(v, &start);
    v++;
    num(v, &end);
    v++;
    num(v, &freq_out);
    v++;
    //MSG("freq_stat:  %s", v[0]);
    num(v, &freq_stat);
    v++;

    *p = v;
    return CO_OK;
}

static int
force(He * he, const real * x, const real * y, const real * z, real * fx,
      real * fy, real * fz)
{
    int i;

    i = 0;
    while (Fo[i]) {
        force_force(Fo[i], he, x, y, z, fx, fy, fz);
        i++;
    }
    return CO_OK;
}


static int
fin(void)
{
    int i;

    i = 0;
    while (Fo[i]) {
        force_fin(Fo[i]);
        i++;
    }
    return CO_OK;
}

static int
F(__UNUSED real t, const real * x, const real * y, const real * z,
  real * vx, real * vy, real * vz, __UNUSED void *p0)
{

    int i, k;
    real coef, al, be;
    real d;
    real dd, ratio;

    coef = 2.0 / (1.0 + lambda);
    al = -2 / (eta * (1.0 + lambda));
    be = 2 * (1 - lambda) / (1 + lambda);

    array_zero3(nv, fx, fy, fz);
    force(he, x, y, z, fx, fy, fz);
    bi_update(bi, he, x, y, z);

    array_zero3(nv, vx, vy, vz);
    for (i = 0; i < nv; i++)
        vx[i] += coef * gamdot * z[i];
    bi_single(bi, he, al, x, y, z, fx, fy, fz, vx, vy, vz);

    //inner and outer viscosity has obvious contrast
    if (1 - lambda > tol || 1 - lambda < -tol) {
        array_zero3(nv, ux, uy, uz);
        for (i = 0; i < nv; i++)
            ux[i] += coef * gamdot * z[i];

        for (k = 1; k <= iter_max; k++) {

            array_copy3(nv, vx, vy, vz, wx, wy, wz);
            bi_double(bi, he, be, x, y, z, ux, uy, uz, wx, wy, wz);
            //vector_tensor(nv, be, ux, uy, uz, Kxx, Kxy, Kxz, Kyy, Kyz, Kzz, wx, wy, wz);

            d = array_msq_3d(nv, ux, uy, uz);
            dd = array_l2_3d(nv, wx, ux, wy, uy, wz, uz);
            ratio = dd / d;

            if (ratio < tol) {

                break;

            }

            array_copy3(nv, wx, wy, wz, ux, uy, uz);
        }

        if (k == iter_max) {
            //MSG("t d dd ratio k = %g %g %g %g %i", t, d, dd, ratio, k);
            if ((fm = fopen(file_msg, "a")) == NULL) {
                ER("Failed to open '%s'", file_msg);
            }

            fprintf(fm, "t d dd ratio k = %g %g %g %g %i\n", t, d, dd,
                    ratio, k);
            fclose(fm);
        }

        array_copy3(nv, ux, uy, uz, vx, vy, vz);

    }                           // if inner and outer viscosity has obvious contrast


    array_zero3(nv, Vx, Vy, Vz);
    he_f_volume_force(fvolume, he, x, y, z, Vx, Vy, Vz);
    array_axpy3(nv, -dt, Vx, Vy, Vz, vx, vy, vz);

    return CO_OK;
}


static real
reduced_volume(real area, real volume)
{
    return (6 * sqrt(pi) * volume) / pow(area, 3.0 / 2);
}

int
main(__UNUSED int argc, char **argv)
{
    real *x, *y, *z;
    Ode3 *ode;
    real t, time;
    int s, i, cnt, j;
    real eng, et, ega, ev, eb, ebl, ebn;
    char name[99];
    real A0, V0, v0;
    real A, V, v;
    real a, e, reg;

    //err_set_ignore();
    argv++;
    y_inif(stdin, &he, &x, &y, &z);
    nv = he_nv(he);
    nt = he_nt(he);
    fargv(&argv, he);

    if ((fm = fopen(file_stat, "w")) == NULL) {
        ER("Failed to open '%s'", file_stat);
    }

    fputs("#dt s t A/A0 V/V0 v et ega ev eb ebl ebn\n", fm);
    fclose(fm);

    V0 = A0 = 1.0;
    i = 0;
    while (Fo[i]) {
        strcpy(name, force_name(Fo[i]));

        if (strcmp(name, "garea") == 0) {
            A0 = he_f_garea_A0(force_pointer(Fo[i]));
        } else if (strcmp(name, "volume") == 0) {
            V0 = he_f_volume_V0(force_pointer(Fo[i]));
        }
        i++;
    }

    v0 = reduced_volume(A0, V0);
    a = A0 / nt;
    e = 2 * sqrt(a) / sqrt(sqrt(3.0));

    if ((fm = fopen(file_msg, "w")) == NULL) {
        ER("Failed to open '%s'", file_msg);
    }

    fprintf(fm, "A0 V0 v0 = %g %g %g\n", A0, V0, v0);
    fprintf(fm, "R D rho eta lambda gamdot = %g %g %g %g\n", rho, eta,
            lambda, gamdot);
    fprintf(fm, "Nv Nt = %i %i\n", nv, nt);
    fprintf(fm, "M m a e dt = %g %g %g\n", a, e, dt);
    fclose(fm);

    ode3_ini(RKF45, nv, dt, F, NULL, &ode);

    CALLOC3(nv, &ux, &uy, &uz);
    CALLOC3(nv, &wx, &wy, &wz);
    CALLOC3(nv, &fx, &fy, &fz);

    t = time = start * dt;
    s = start;

    while (1) {

        if (s % freq_out == 0) {
            sprintf(file_out, "%07d.off", s);
            off_he_xyz_write(he, x, y, z, file_out);
        }

        if (s % freq_stat == 0) {

            eng = 0.0;
            ega = 0.0;
            ev = 0.0;
            eb = 0.0;
            ebl = 0.0;
            ebn = 0.0;
            A = 0.0;
            V = 0.0;
            v = 0.0;

            i = 0;
            et = 0.0;
            while (Fo[i]) {

                strcpy(name, force_name(Fo[i]));
                eng = force_energy(Fo[i], he, x, y, z);
                et += eng;

                if (strcmp(name, "garea") == 0) {
                    ega = eng;
                    A = he_f_garea_A(force_pointer(Fo[i]));
                } else if (strcmp(name, "volume") == 0) {
                    ev = eng;
                    V = he_f_volume_V(force_pointer(Fo[i]));
                } else if (strcmp(name, "juelicher_xin") == 0) {
                    eb = eng;
                    ebl =
                        he_f_juelicher_xin_energy_bend(force_pointer
                                                       (Fo[i]));
                    ebn =
                        he_f_juelicher_xin_energy_ad(force_pointer(Fo[i]));

                }

                i++;
            }

            v = reduced_volume(A, V);

            /*if ( s / 10 % freq_stat == 0 ) {
               MSG("dt s t = %g %i %g", dt, s, t);
               MSG("A/A0 V/V0 v  = %g %g %g", A/A0, V/V0, v);
               MSG("et ega ev eb ebl ebn = %g %g %g %g %g %g", et, ega, ev, eb, ebl, ebn);
               } */

            if ((fm = fopen(file_stat, "a")) == NULL) {
                ER("Failed to open '%s'", file_stat);
            }

            fprintf(fm, "%g %i %g %g %g %g %g %g %g %g %g %g\n", dt, s, t,
                    A / A0, V / V0, v, et, ega, ev, eb, ebl, ebn);
            fclose(fm);

        }

        if (s % 100 == 0) {

            j = 0;
            do {
                he_equiangulate(he, x, y, z, &cnt);
                //MSG("cnt : %d", cnt);
                j++;
            } while (cnt > 0 && j < 10);

        }


        s++;
        t = time + dt;

        if (s > end)
            break;
        ode3_apply(ode, &time, t, x, y, z);
	y_tocm(he, x, y, z);
    }
    FREE3(Vx, Vy, Vz);
    FREE3(ux, uy, uz);
    FREE3(wx, wy, wz);
    FREE3(fx, fy, fz);
    ode3_fin(ode);
    bi_fin(bi);
    fin();
    y_fin(he, x, y, z);
    he_f_volume_fin(fvolume);
}
