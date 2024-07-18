#include <complex.h>
#include <math.h>
#include "fractal_functions.h"

// Returns the iterations required before the
// sequence where z_{n+1} = z_n^2 + c diverges
// (where |z| > 2). Returns a fractalOut struct:
// iters is -1 if z_n does not diverge before MAX_ITERS.
fractalOut multibrot(int P, double complex c, int max_iters) {
    double complex z = 0;
    int n = 0;
    while (cabs(z) < BAILOUT_RADIUS) {
        z = cpow(z, P) + c;
        n++;
        if (n > max_iters) {
            n = -1;
            break;
        }
    }
    double logEnd = log(cabs(z));
    double smoothIters = n + 1 - (log(logEnd / log(2)) / log(P));
    fractalOut out = {
        .iters = n, .smoothIters = smoothIters,
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
fractalOut multijulia(int P, double complex z, double complex c, int max_iters) {
    int n = 0;
    fractalOut out;
    out.start = z;
    while (cabs(z) < BAILOUT_RADIUS) {
        z = cpow(z, P) + c;
        n++;
        if (n > max_iters) {
            n = -1;
            break;
        }
    }
    double logEnd = log(cabs(z));
    out.smoothIters = n + 1 - (log(logEnd / log(2)) / log(P));
    out.iters = n;
    out.end = z;
    return out;
}

fractalOut mandelbrot(double complex c, int max_iters) {
    return multibrot(2, c, max_iters); 
}

fractalOut julia(double complex z, double complex c, int max_iters) {
    return multijulia(2, z, c, max_iters); 
}
