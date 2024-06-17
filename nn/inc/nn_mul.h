#ifndef __NN_MUL_H
#define __NN_MUL_H

#include <assert.h>
#include <float.h>

#include "nn_tensor.h"
#include "ops/mul.h"
#include "ops/mul1.h"


/**
 * Returns the element-wise multiplication of two tensors.
 * 
 * out_i = a_i * b_i
 * 
 * @param out: the output tensor
 * @param a: the input tensor
 * @param b: the input tensor
 */
void NN_mul(Tensor *out, Tensor *a, Tensor *b);

/**
 * Returns the element-wise multiplication of the input tensor with a scalar.
 * 
 * @param out: the output tensor
 * @param in: the input tensor
 * @param scalar: scalar value
 */
void NN_mul1(Tensor *out, Tensor *in, float scalar);


#endif // __NN_MUL_H
