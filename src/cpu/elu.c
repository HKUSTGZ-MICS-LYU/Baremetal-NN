#include "nn.h"


__attribute__((weak)) void NN_elu2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x, float alpha) {
  NN_assert(x->shape[0] == y->shape[0] && x->shape[1] == y->shape[1], "Cannot perform ReLU on tensors of different shapes");

  const size_t n = x->shape[0] * x->shape[1];
  for (size_t i = 0; i < n; i += 1) {
    if (x->data[i] > 0) {
      y->data[i] = x->data[i];
    }
    else {
      y->data[i] = alpha * (expf(x->data[i]) - 1.f);
    }
  }
}

