#ifndef __NN_LAYER_NORM_H
#define __NN_LAYER_NORM_H

#include <assert.h>
#include <math.h>

#include "nn_tensor.h"
#include "sum.h"
#include "add.h"
#include "add1.h"
#include "mul.h"
#include "mul1.h"
#include "sqr.h"


void NN_layer_norm(
  Tensor *out, Tensor *in,
  size_t normalized_dims,
  Tensor *weight, Tensor *bias,
  const float eps);


#endif // __NN_LAYER_NORM_H
