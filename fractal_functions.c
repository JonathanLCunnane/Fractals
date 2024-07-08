#include <complex.h>
#include "fractal_functions.h"

// Returns the iterations required before the
// sequence where z_{n+1} = z_n^2 + c diverges
// (where |z| > 2). Returns -1 if z_n does not
// diverge before MAX_ITERS.
int mandelbrotIters(double complex c) {
    double complex z = 0;
    int n = 0;
    while (cabs(z) < 2) {
        z = cpow(z, 2) + c;
        n++;
        if (n > MAX_ITERS) return -1;
    }
    return n;
}

// Returns the iterations required before
// the complex number z diverges (where |z| > 2)
// under the repeated application of the following
// map: f(z) = z^P - c.
// Returns -1 if z does not diverge before MAX_ITERS.
int juliaIters(int P, double complex z, double complex c) {
    int n = 0;
    while (cabs(z) < 2) {
        z = cpow(z, P) + c;
        n++;
        if (n > MAX_ITERS) return -1;
    }
    return n;
}

