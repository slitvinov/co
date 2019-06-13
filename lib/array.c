#include <stdio.h>
#include "real.h"
#include "co/err.h"
#include "co/array.h"

#define OUT CO_REAL_OUT

real
array_min(int n, const real a[])
{
	int i;
	real x;
	for (i = 0, x = a[0]; i < n; i++)
		if (a[i] < x) x = a[i];
	return x;
}

real
array_max(int n, const real a[])
{
	int i;
	real x;
	for (i = 0, x = a[0]; i < n; i++)
		if (a[i] > x) x = a[i];
	return x;
}

int
array_zero(int n, real a[])
{
	int i;
	if (n < 0)
		ERR(CO_INDEX, "n=%d < 0", n);
	for (i = 0; i < n; i++)
		a[i] = 0;
	return CO_OK;
}

int
array_one(int n, real a[])
{
	int i;
	if (n < 0)
		ERR(CO_INDEX, "n=%d < 0", n);
	for (i = 0; i < n; i++)
		a[i] = 1;
	return CO_OK;
}

int
array_zero3(int n, real a[], real b[], real c[])
{
	if (n < 0)
		ERR(CO_INDEX, "n=%d < 0", n);
	array_zero(n, a);
	array_zero(n, b);
	array_zero(n, c);
	return CO_OK;
}

int
array_axpy(int n, real a, const real x[], /**/ real y[])
{
	int i;
	for (i = 0; i < n; i++)
		y[i] += a * x[i];
	return CO_OK;
}

int
array_axpy3(int n, real a,
const real x0[], const real x1[], const real x2[],
real y0[], real y1[], real y2[])
{
	array_axpy(n, a, x0, y0);
	array_axpy(n, a, x1, y1);
	array_axpy(n, a, x2, y2);
	return CO_OK;
}

int
array_copy(int n, const real* a, real *b)
{
	int i;
	for (i = 0; i < n; i++)
		b[i] = a[i];
	return CO_OK;
}

int
array_copy3(int n, const real *x, const real *y, const real *z, real *u, real *v, real *w)
{
	array_copy(n, x, u);
	array_copy(n, y, v);
	array_copy(n, z, w);
	return CO_OK;
}

int
array_negative(int n, const real* a, real *b)
{
	int i;
	for (i = 0; i < n; i++)
		b[i] = -a[i];
	return CO_OK;
}

int
array_neg(int n, real *a)
{
	int i;
	for (i = 0; i < n; i++)
		a[i] = -a[i];
	return CO_OK;
}

real
array_dot(int n, const real *a, const real *b)
{
	int i;
	real s;
	s = 0;
	for (i = 0; i < n; i++)
		s += a[i] * b[i];
	return s;
}

real
array_mean(int n, const real *a)
{
	int i;
	real s;
	s = 0;
	for (i = 0; i < n; i ++)
		s += a[i];
	return s/n;
}

int
array_scale(int n, real s, real *a)
{
	int i;
	for (i = 0; i < n; i++)
		a[i] *= s;
	return CO_OK;
}

int
array_scale3(int n, real s, real *x, real *y, real *z)
{
	array_scale(n, s, x);
	array_scale(n, s, y);
	array_scale(n, s, z);
	return CO_OK;
}

int
array_fwrite(FILE *f, int n, const real *a)
{
	int i, status;
	for (i = 0; i < n; i++) {
		status = fprintf(f, OUT "\n", a[i]);
		if (status < 0)
			ERR(CO_IO, "fprintf failed");
	}
	return CO_OK;
}
