#ifndef FRACTAL_FUNCTIONS_H
#define FRACTAL_FUNCTIONS_H

#define MAX_ITERS 20
#define BAILOUT_RADIUS (1 << 16)

typedef struct {
    double complex start;
    double complex end;
    int iters;
} fractalOut;

extern fractalOut mandelbrot(double complex);
extern fractalOut julia(int, double complex, double complex);

#endif
