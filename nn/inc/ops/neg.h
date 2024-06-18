#ifndef __NN__NEG_H
#define __NN__NEG_H

#include <stddef.h>
#include <stdint.h>

#ifdef RVV
  #include <riscv_vector.h>
#endif

static inline void NN__neg_F32(size_t n, float *y, float *x)  {
  #ifdef RVV
    while (n > 0) {
      size_t vl = __riscv_vsetvl_e32m1(n);
      vfloat32m1_t vec_x = __riscv_vle32_v_f32m1(x, vl);
      vfloat32m1_t vec_y = __riscv_vfneg_v_f32m1(vec_x, vl);
      __riscv_vse32_v_f32m1(y, vec_y, vl);
      x += vl;
      y += vl;
      n -= vl;
    }
  #else
    for (size_t i = 0; i < n; i += 1) {
      y[i]  = -x[i];
    }
  #endif
}


#endif // __NN__NEG_H
