#define T

typedef struct T T;

int ring_ini(/**/ T**);

/* vertice, ring[], x, y, z */
int ring_alpha(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_beta(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_theta(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_xyz(T*, int, const int*, const real*, const real*, const real*, /**/ real**);
int ring_A(T*, int, const int*, const real*, const real*, const real*, /**/ real**);

int ring_fin(T);

#undef T
