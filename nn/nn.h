#ifndef __NN_H
#define __NN_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "float16.h"


// http://elm-chan.org/junk/32bit/binclude.html
#define INCLUDE_FILE(section, filename, symbol) asm (\
    ".section "#section"\n"                   /* Change section */\
    ".balign 4\n"                             /* Word alignment */\
    ".global "#symbol"_start\n"               /* Export the object start address */\
    ".global "#symbol"_data\n"                /* Export the object address */\
    #symbol"_start:\n"                        /* Define the object start address label */\
    #symbol"_data:\n"                         /* Define the object label */\
    ".incbin \""filename"\"\n"                /* Import the file */\
    ".global "#symbol"_end\n"                 /* Export the object end address */\
    #symbol"_end:\n"                          /* Define the object end address label */\
    ".balign 4\n"                             /* Word alignment */\
    ".section \".text\"\n")                   /* Restore section */


typedef struct {
  float16_t data;
} Tensor0D_F16;

typedef struct {
  float data;
} Tensor0D_F32;

typedef struct {
  size_t shape[1];
  float16_t *data;
} Tensor1D_F16;

typedef struct {
  size_t shape[1];
  float *data;
} Tensor1D_F32;

typedef struct {
  size_t shape[2];
  float16_t *data;
} Tensor2D_F16;

typedef struct {
  size_t shape[2];
  float *data;
} Tensor2D_F32;


static inline void nn_assert(int condition, char *message) {
  if (!condition) {
    printf("Assertion failed: ");
    printf("%s\n", message);
    exit(1);
  }
}

static inline uint8_t float_equal(float golden, float actual, float rel_err) {
  return (fabs(actual - golden) < rel_err) || (fabs((actual - golden) / actual) < rel_err);
}




void nn_print_u8(uint8_t v);

void nn_print_i8(int8_t v);

void nn_print_u16(uint16_t v);

void nn_print_i16(int16_t v);

void nn_print_u32(uint32_t v);

void nn_print_i32(int32_t v);

void nn_print_f16(float16_t v, int16_t num_digits);

void nn_print_f32(float v, int16_t num_digits);

void nn_print_shape(size_t ndim, const size_t *shape);

void nn_print_tensor1d_f16(const Tensor1D_F16 *tensor);

void nn_print_tensor1d_f32(const Tensor1D_F32 *tensor);

void nn_print_tensor2d_f16(const Tensor2D_F16 *tensor);

void nn_print_tensor2d_f32(const Tensor2D_F32 *tensor);


uint8_t nn_equals0d_f16(const Tensor0D_F16 *a, const Tensor0D_F16 *b, float rel_err);

uint8_t nn_equals0d_f32(const Tensor0D_F32 *a, const Tensor0D_F32 *b, float rel_err);

uint8_t nn_equals1d_f16(const Tensor1D_F16 *a, const Tensor1D_F16 *b, float rel_err);

uint8_t nn_equals1d_f32(const Tensor1D_F32 *a, const Tensor1D_F32 *b, float rel_err);

uint8_t nn_equals2d_f16(const Tensor2D_F16 *a, const Tensor2D_F16 *b, float rel_err);

uint8_t nn_equals2d_f32(const Tensor2D_F32 *a, const Tensor2D_F32 *b, float rel_err);



void nn_add1d_f16(Tensor1D_F16 *y, const Tensor1D_F16 *x1, const Tensor1D_F16 *x2);

void nn_add1d_f32(Tensor1D_F32 *y, const Tensor1D_F32 *x1, const Tensor1D_F32 *x2);

void nn_add2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x1, const Tensor2D_F16 *x2);

void nn_add2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x1, const Tensor2D_F32 *x2);

void nn_addscalar1d_f16(Tensor1D_F16 *y, const Tensor1D_F16 *x, float16_t scalar);

void nn_addscalar1d_f32(Tensor1D_F32 *y, const Tensor1D_F32 *x, float scalar);

void nn_addscalar2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x, float16_t scalar);

void nn_addscalar2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x, float scalar);


void nn_addmm_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x, const Tensor2D_F16 *weight, const Tensor1D_F16 *bias);

void nn_addmm_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x, const Tensor2D_F32 *weight, const Tensor1D_F32 *bias);



void nn_elu2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x, float alpha);

void nn_elu2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x, float alpha);



void nn_relu2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x);

void nn_relu2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x);


void nn_max1d_f16(Tensor0D_F16 *y, const Tensor1D_F16 *x);

void nn_max1d_f32(Tensor0D_F32 *y, const Tensor1D_F32 *x);

void nn_max2d_f16(Tensor0D_F16 *y, const Tensor2D_F16 *x);

void nn_max2d_f32(Tensor0D_F32 *y, const Tensor2D_F32 *x);


void nn_min1d_f16(Tensor0D_F16 *y, const Tensor1D_F16 *x);

void nn_min1d_f32(Tensor0D_F32 *y, const Tensor1D_F32 *x);

void nn_min2d_f16(Tensor0D_F16 *y, const Tensor2D_F16 *x);

void nn_min2d_f32(Tensor0D_F32 *y, const Tensor2D_F32 *x);


void nn_mm_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x1, const Tensor2D_F16 *x2);

void nn_mm_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x1, const Tensor2D_F32 *x2);


void nn_mul1d_f16(Tensor1D_F16 *y, const Tensor1D_F16 *x1, const Tensor1D_F16 *x2);

void nn_mul1d_f32(Tensor1D_F32 *y, const Tensor1D_F32 *x1, const Tensor1D_F32 *x2);

void nn_mul2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x1, const Tensor2D_F16 *x2);

void nn_mul2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x1, const Tensor2D_F32 *x2);

void nn_mulscalar1d_f16(Tensor1D_F16 *y, const Tensor1D_F16 *x, float16_t scalar);

void nn_mulscalar1d_f32(Tensor1D_F32 *y, const Tensor1D_F32 *x, float scalar);

void nn_mulscalar2d_f16(Tensor2D_F16 *y, const Tensor2D_F16 *x, float16_t scalar);

void nn_mulscalar2d_f32(Tensor2D_F32 *y, const Tensor2D_F32 *x, float scalar);








#endif // __NN_H