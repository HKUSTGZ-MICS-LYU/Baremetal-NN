#ifndef __NN__ADD_H
#define __NN__ADD_H

#include <stddef.h>
#include <stdint.h>

#include "nn_float16.h"


void NN__add_i8(size_t n,
    int8_t *z, size_t incz,
    int8_t *x, size_t incx,
    int8_t *y, size_t incy
    );

void NN__add_f16(size_t n,
    float16_t *z, size_t incz,
    float16_t *x, size_t incx,
    float16_t *y, size_t incy
    );

void NN__add_f32(size_t n,
    float *z, size_t incz,
    float *x, size_t incx,
    float *y, size_t incy
    );


#endif // __NN__ADD_H
