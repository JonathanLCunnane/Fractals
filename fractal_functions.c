#include <complex.h>
#include "fractal_functions.h"

// Returns the iterations required before the
// sequence where z_{n+1} = z_n^2 + c diverges
// (where |z| > 2). Returns a fractalOut struct:
// iters is -1 if z_n does not diverge before MAX_ITERS.
fractalOut mandelbrot(double complex c) {
    double complex z = 0;
    int n = 0;
    while (cabs(z) < BAILOUT_RADIUS) {
        z = cpow(z, 2) + c;
        n++;
        if (n > MAX_ITERS) {
            n = -1;
            break;
        }
    }
    fractalOut out = {
        .iters = n,
        .start = c,
        .end   = z
    };
    return out;
}

// Returns the iterations required before
// the complex number z diverges (where |z| > 2)
// under the repeated application of the following
// map: f(z) = z^P - c.
// Returns a fractalOut struct:
// iters is -1 if z does not diverge before MAX_ITERS.
fractalOut julia(int P, double complex z, double complex c) {
    int n = 0;
    fractalOut out;
    out.start = z;
    while (cabs(z) < BAILOUT_RADIUS) {
        z = cpow(z, P) + c;
        n++;
        if (n > MAX_ITERS) {
            n = -1;
            break;
        }
    }
    out.iters = n;
    out.end = z;
    return out;
}

