#include "impl/mul.h"


__attribute__((weak)) void NN_mul_i8(size_t n, int8_t *z, size_t incz, const int8_t *x, size_t incx, const int8_t *y, size_t incy) {
  for (size_t i = 0; i < n; i += 1) {
    z[i * incz] = x[i * incx] * y[i * incy];
  }
}

__attribute__((weak)) void NN_mul_i16(size_t n, int16_t *z, size_t incz, const int16_t *x, size_t incx, const int16_t *y, size_t incy) {
  for (size_t i = 0; i < n; i += 1) {
    z[i * incz] = x[i * incx] * y[i * incy];
  }
}

__attribute__((weak)) void NN_mul_i32(size_t n, int32_t *z, size_t incz, const int32_t *x, size_t incx, const int32_t *y, size_t incy) {
  for (size_t i = 0; i < n; i += 1) {
    z[i * incz] = x[i * incx] * y[i * incy];
  }
}

__attribute__((weak)) void NN_mul_f16(size_t n, float16_t *z, size_t incz, const float16_t *x, size_t incx, const float16_t *y, size_t incy) {
  for (size_t i = 0; i < n; i += 1) {
    z[i * incz] = NN_float_to_half(NN_half_to_float(x[i * incx]) * NN_half_to_float(y[i * incy]));
  }
}

__attribute__((weak)) void NN_mul_f32(size_t n, float *z, size_t incz, const float *x, size_t incx, const float *y, size_t incy) {
  for (size_t i = 0; i < n; i += 1) {
    z[i * incz] = x[i * incx] * y[i * incy];
  }
}
