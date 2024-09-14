#ifndef __NN_SQR_H
#define __NN_SQR_H

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#include "float16.h"


void NN_sqr_i8(size_t n,
    int8_t *y, size_t incy,
    const int8_t *x, size_t incx
    );

void NN_sqr_i16(size_t n,
    int16_t *y, size_t incy,
    const int16_t *x, size_t incx
    );

void NN_sqr_i32(size_t n,
    int32_t *y, size_t incy,
    const int32_t *x, size_t incx
    );

void NN_sqr_f16(size_t n,
    float16_t *y, size_t incy,
    const float16_t *x, size_t incx
    );

void NN_sqr_f32(size_t n,
    float *y, size_t incy,
    const float *x, size_t incx
    );


#endif // __NN_SQR_H