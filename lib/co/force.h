#define T Force

typedef struct He He;
typedef struct T T;

int force_argv(const char *name, char***, He*, /**/ T**);
int force_fin(T*);
int force_force(T*, He*, const real*, const real*, const real*, /**/ real*, real*, real*);
real force_energy(T*, He*, const real*, const real*, const real*);
const char* force_name(T*);

const char* force_list();
int force_narg(const char*);

#undef T