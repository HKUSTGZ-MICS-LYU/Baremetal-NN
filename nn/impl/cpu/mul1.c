#include "mul1.h"


__attribute__((weak)) void NN__mul1_f32(size_t n, float *y, size_t incy, const float *x, size_t incx, float scalar) {
  for (size_t i = 0; i < n; i += 1) {
    y[i * incy]  = x[i * incx] * scalar;
  }
}
