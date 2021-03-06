#include <stdio.h>
#include <stdlib.h>
#include <real.h>
#include <co/array.h>
#include <co/area.h>
#include <co/argv.h>
#include <co/bi/cortez_zero.h>
#include <co/err.h>
#include <co/he.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/surface.h>
#include <co/vec.h>
#include <co/y.h>

#define FMT   CO_REAL_OUT

static char me[] = "bi/velocity";
static void
usg()
{
    fprintf(stderr, "%s -b lx ly lz hx hy hz -n nx ny nz < OFF > VTK\n",
            me);
    exit(1);
}

int
main(int argc, char **argv)
{
    enum { X, Y, Z };
    int i, j, k, l, nv, n;
    Surface *surface;
    BiCortezZero *cortez;
    He *he;
    real *x, *y, *z, *ux, *uy, *uz, *vx, *vy, *vz, *area, *distance;
    real lx, ly, lz, hx, hy, hz, dx, dy, dz, r[3], v[3];
    int nx, ny, nz, Bset, Nset;
    FILE *f;
    real lo[2] = { -3, -3 }, hi[2] = { 3, 3 }, size = 0.25;

    //err_set_ignore();
    USED(argc);
    argv++;
    Bset = Nset = 0;
    while (*argv != NULL && argv[0][0] == '-')
        switch (argv[0][1]) {
        case 'h':
            usg();
            break;
        case 'b':
            argv++;
            argv_real(&argv, &lx);
            argv_real(&argv, &ly);
            argv_real(&argv, &lz);
            argv_real(&argv, &hx);
            argv_real(&argv, &hy);
            argv_real(&argv, &hz);
            Bset = 1;
            break;
        case 'n':
            argv++;
            argv_int(&argv, &nx);
            argv_int(&argv, &ny);
            argv_int(&argv, &nz);
            Nset = 1;
            break;
        default:
            ER("unknown option '%s'\n", argv[0]);
            break;
        }
    if (!Nset)
        ER("needs -n option");
    if (!Bset)
        ER("needs -b option");

    dx = nx == 0 ? 0 : (hx - lx) / nx;
    dy = ny == 0 ? 0 : (hy - ly) / ny;
    dz = nz == 0 ? 0 : (hz - lz) / nz;
    n = (nx + 1) * (ny + 1) * (nz + 1);

    y_inif(stdin, &he, &x, &y, &z);
    surface_ini(lo, hi, size, &surface);
    surface_update(surface, he, x, y, z);

    bi_cortez_zero_ini(he, &cortez);
    nv = he_nv(he);
    MALLOC3(nv, &ux, &uy, &uz);
    MALLOC3(n, &vx, &vy, &vz);
    MALLOC(n, &distance);
    MALLOC(nv, &area);
    he_area_ver(he, x, y, z, area);
    for (i = 0; i < nv; i++) {
        ux[i] = 1;
        uy[i] = 2;
        uz[i] = 3;
    }
    l = 0;
    for (k = 0; k <= nz; k++)
        for (j = 0; j <= ny; j++)
            for (i = 0; i <= nx; i++) {
                r[X] = lx + dx * i;
                r[Y] = ly + dy * j;
                r[Z] = lz + dz * k;
                bi_cortez_zero_double_velocity(cortez, he, x, y, z, ux, uy,
                                               uz, r, v);
                vx[l] = v[X];
                vy[l] = v[Y];
                vz[l] = v[Z];
                surface_distance(surface, r[X], r[Y], r[Z], &distance[l]);
                l++;
            }
    f = stdout;
    fprintf(f, "# vtk DataFile Version 2.0\n"
            "generated by %s\n"
            "ASCII\n"
            "DATASET STRUCTURED_POINTS\n"
            "DIMENSIONS %d %d %d\n"
            "ORIGIN " FMT " " FMT " " FMT "\n"
            "SPACING " FMT " " FMT " " FMT "\n",
            me, nx + 1, ny + 1, nz + 1, lx, ly, lz, dx, dy, dz);
    fprintf(f, "POINT_DATA %d\n", n);
    fputs("VECTORS v double\n", f);
    for (i = 0; i < n; i++)
        fprintf(f, FMT " " FMT " " FMT "\n", vx[i], vy[i], vz[i]);
    fputs("SCALARS distance double\n", f);
    fputs("LOOKUP_TABLE DEFAULT\n", f);
    for (i = 0; i < n; i++)
        fprintf(f, FMT "\n", distance[i]);

    surface_fin(surface);
    y_fin(he, x, y, z);
    FREE(area);
    FREE(distance);
    FREE3(ux, uy, uz);
    FREE3(vx, vy, vz);
    bi_cortez_zero_fin(cortez);
}
