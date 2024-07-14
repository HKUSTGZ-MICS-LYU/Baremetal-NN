#ifndef __NN__MIN_H
#define __NN__MIN_H

#include <stddef.h>
#include <stdint.h>
#include <float.h>


void NN__min_f32(size_t n,
    float *result,
    const float *x, size_t incx
    );


#endif // __NN__MIN_H
