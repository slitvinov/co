real tri_angle(const real[3], const real b[3], const real[3]);
real tri_area(const real[3], const real[3], const real[3]);
real tri_cot(const real[3], const real b[3], const real[3]);
real tri_dih(const real[3], const real[3], const real[3], const real[3]);
int  tri_edg(const real[3], const real[3], const real[3], /**/ real[3], real[3], real[3]);
real tri_volume(const real[3], const real[3], const real[3]);
int  tri_normal(const real[3], const real[3], const real[3], /**/ real[3]);
int tri_center(const real[3], const real[3], const real[3], /**/ real[3]);

/* geomveiw objects */
int tri_off(const real[3], const real[3], const real[3], FILE*);
int tri_vect(const real[3], const real[3], const real[3],
             const real[3], const real[3], const real[3],
             FILE*);
int tri_list(const real[3], const real[3], const real[3],
             const real[3], const real[3], const real[3],
             FILE*);

/* project triangle to 2D:  a->[0, 0],  b->[ux, uy = 0],  c->[wx, uy] */
int tri_3to2(const real a[3], const real b[3], const real c[3], /**/
             real *ux, real *uy, real *wx, real *wy);

/* ex[3], ey[3]: basis local to a triangle
   ex = norm(b - a)
   ey is normal to ex in a plate of a triangle
*/
int tri_2to3(const real a[3], const real b[3], const real c[3], /**/
             real ex[3], real ey[3]);
