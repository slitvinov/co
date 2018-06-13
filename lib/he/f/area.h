#define T HeFArea

typedef struct He He;
typedef struct T T;

/* E = K * sum_{tri} (area_{tri} - area0)^2 */
int he_f_area_ini(real a0, real K, He*, T**);
int he_f_area_fin(T*);

int he_f_area_force(T*, He*,
                    const real *x, const real *y, const real *z, /**/
                    real *fx, real *fy, real *fz);
real he_f_area_energy(T*, He*, const real *x, const real *y, const real *z);

int he_f_area_a(T*, /**/ real  **a);
int he_f_area_da(T*, /**/ real **da);

#undef T
