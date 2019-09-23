static real
i_vec_dot(const real a[3], const real b[3])
{
    enum {X, Y, Z};
    return a[X]*b[X] + a[Y]*b[Y] + a[Z]*b[Z];
}

static real
i_vec_abs(const real a[3])
{
    return sqrt(i_vec_dot(a, a));
}

static int
i_vec_get(int i, const real x[], const real y[], const real z[], /**/ real a[3]) {
    enum {X, Y, Z};
    a[X] = x[i]; a[Y] = y[i]; a[Z] = z[i];
    return CO_OK;
}

static int
i_vec_minus(const real a[3], const real b[3], /**/ real c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] - b[X];
    c[Y] = a[Y] - b[Y];
    c[Z] = a[Z] - b[Z];
    return CO_OK;
}

static int
i_vec_plus(const real a[3], const real b[3], /**/ real c[3]) {
    enum {X, Y, Z};
    c[X] = a[X] + b[X];
    c[Y] = a[Y] + b[Y];
    c[Z] = a[Z] + b[Z];
    return CO_OK;
}
