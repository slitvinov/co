#define T HeFCahnman

typedef struct He He;
typedef struct T T;

int he_f_cahnman_ini(real K, real C0, real Kad, He*, T**);
int he_f_cahnman_fin(T*);

int he_f_cahnman_force(T*, He*,
                      const real *x, const real *y, const real *z, /**/
                      real *fx, real *fy, real *fz);
real he_f_cahnman_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_cahnman_curva_mean(T*, /**/ real**);
int he_f_cahnman_energy_ver(T*, /**/ real**);
int he_f_cahnman_area_ver(T*, /**/ real**);
int he_f_cahnman_fad(T*, /**/ real**, real**, real**);

#undef T
