#define T Oseen2
typedef struct T T;
typedef struct Skel Skel;
int oseen2_ini(real, T**);
int oseen2_fin(T*);
int oseen2_apply(T*, Skel*, const real*, const real*, real *xx, real *xy, real *yy);
#undef T