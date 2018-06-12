#define T HeFArea

typedef struct He He;
typedef struct T T;

int he_f_area_ini(real a0, real k, He*, T**);
int he_f_area_fin(T*);

int he_f_area_compute(T*, He*,
                      real *x, real *y, real *z, /**/
                      real *fx, real *fy, real *fz);

int he_f_area_a(T*, /**/ real  **a);
int he_f_area_da(T*, /**/ real **da);

#undef T
