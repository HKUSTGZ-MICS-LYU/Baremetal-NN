#ifndef __NN_CLIP_H
#define __NN_CLIP_H

#include <assert.h>

#include "nn_tensor.h"
#include "ops/maximum1.h"
#include "ops/minimum1.h"


/**
 * Clamps all elements in input into the range [ min, max ]. 
 * Letting min_value and max_value be min and max, respectively, this returns:
 * 
 *   y_i = min(max(x_i, min_value), max_value)
 * 
 * @param out: the output tensor
 * @param a: the input tensor
 * @param min: lower-bound of the range to be clamped to
 * @param max: upper-bound of the range to be clamped to
 */
void NN_clip(Tensor *y, Tensor *x, float min, float max);

void NN_clipInplace(Tensor *x, float min, float max);


#endif // __NN_CLIP_H
