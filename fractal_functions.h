#ifndef FRACTAL_FUNCTIONS_H
#define FRACTAL_FUNCTIONS_H

#define BAILOUT_RADIUS (1 << 16)

typedef struct {
    double complex start;
    double complex end;
    int iters;
    double smoothIters;
} fractalOut;

extern fractalOut mandelbrot(double complex, int);
extern fractalOut julia(int, double complex, double complex, int);

#endif
