#include "mandelbrot.hpp"

int mandelbrot(long double real, long double imaginary, int maxIterations) {
    long double zr = 0;
    long double zi = 0;

    for (int i = 0; i < maxIterations; i++) {
        long double zr2 = zr * zr;
        long double zi2 = zi * zi;

        if (zr2 + zi2 > 4.0) {
            return i;
        }

        long double zi_new = 2.0 * zr * zi + imaginary;
        zr = zr2 - zi2 + real;
        zi = zi_new;
    }

    return maxIterations;
}