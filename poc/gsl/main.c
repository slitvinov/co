#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>

#include <he/err.h>
#include <he/macro.h>

#include "sys.h"

int main(void) {
    gsl_odeiv2_system sys = {f, NULL, DIM, &param};
    gsl_odeiv2_driver *driver;
    int i;
    double t, t1;
    double ti;
    int n, status;
    double istep, rel, abs;

    istep = 1e-12; /* initial step size */
    rel = 1e-16;
    abs = 1e-20;
    n = 40000;
    t = 0.0, t1 = 1200.0;
    ini();
    
    driver = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkf45,
                                       istep, rel, abs);
    for (i = 1; i <= n; i++) {
        ti = i * t1 / n;
        status = gsl_odeiv2_driver_apply(driver, &t, ti, q);
        if (status != GSL_SUCCESS)
            ER("[gsl] error, return value=%d\n", status);
        printf("%.5e ", t);
        print(stdout, "%.5e");
        putc('\n', stdout);
    }
    gsl_odeiv2_driver_free(driver);
    return 0;
}
