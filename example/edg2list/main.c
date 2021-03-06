#include <stdio.h>

#include <real.h>

#include <co/array.h>
#include <co/bbox2.h>
#include <co/edg2.h>
#include <co/err.h>
#include <co/skel.h>
#include <co/list/edg2.h>
#include <co/macro.h>
#include <co/memory.h>
#include <co/vec2.h>
#include <co/off.h>
#include <co/y.h>

#define FMT CO_REAL_OUT
enum 
{
	X, Y
};

int
main(__UNUSED int argc, const char **argv)
{
	Skel *skel;
	Edg2List *list;
	Bbox2 *bbox;
	real *x, *y, *color;
	real *lo, *hi;
	real size = 0.1;
	real p[2];
	real point[2], u[2];
	int status, ne, nv, t, j, *edgs;

	err_set_ignore();
	argv++;
	vec2_argv(&argv, p);
	skel_read(stdin,  &x, &y, &skel);
	nv = skel_nv(skel);
	ne = skel_ne(skel);
	bbox2_ini(&bbox);
	bbox2_update(bbox, nv, x, y);
	bbox2_lo(bbox, &lo);
	bbox2_hi(bbox, &hi);

	vec2_ini(size, size, u);
	vec2_add(u, hi);
	vec2_sub(u, lo);

	CALLOC(ne, &color);

	edg2list_ini(lo, hi, size, &list);
	edg2list_push(list, skel, x, y);

	edg2list_get(list, p[X], p[Y]);
	status = edg2list_status(list);
	MSG("status: %d", status);
	if (status) {
		edg2list_edgs(list, &edgs);
		while ( (j = *edgs++) != -1)
			color[j] = 1;
		t = edg2list_edg(list);
		color[t] = 2;
		edg2list_point(list, point);
		fputs("LIST{\n", stdout);
		edg2_vect(p, point, stdout);
		skel_edg_color_write(skel, x, y, color, stdout);
		fputs("}", stdout);
	}

	FREE(color);
	edg2list_fin(list);
	bbox2_fin(bbox);
	skel_xy_fin(x, y, skel);
}

/* 
m; ./main 0.15 -0.2 < data/rbc.skel  > q.off; co.geomview -p cat  -a /u/a q.off 
*/