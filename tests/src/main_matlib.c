#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rv.h>

#include "nn.h"
#include "riscv_vector.h"

#define N_DIMS 2

static void enable_vector_operations() {
    unsigned long mstatus;
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= 0x00000600 | 0x00006000 | 0x00018000;
    asm volatile("csrw mstatus, %0"::"r"(mstatus));
}

uint8_t float_eq(float golden, float actual, float relErr) {
  return (fabs(actual - golden) < relErr) || (fabs((actual - golden) / actual) < relErr);
}

uint8_t compare_2d(float *golden, float *actual, int n, int m) {
  for (int i = 0; i < m * n; i+=1) {
    if (!float_eq(golden[i], actual[i], 1e-6)) {
      return 0;
    }
  }
  return 1;
}


#define M  3
#define N  4
#define O  5

int main() {
  enable_vector_operations();
  
  uint32_t seed = 0xdeadbeef;
  size_t cycles;
  srand(seed);

  // matmul
  {
    Tensor *A = NN_rand(2, (size_t[]){M, O}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){O, N}, DTYPE_F32);
    Tensor *BT = NN_rand(2, (size_t[]){N, O}, DTYPE_F32);
    Tensor *f = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);

    printf("matmul:\t\t");
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    
    NN_matmul_F32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_matmul_F32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);
    
    printf("matmulT:\t");
    
    NN_matmulT_F32(golden, A, BT);
    cycles = READ_CSR("mcycle");
    NN_matmulT_F32_RVV(actual, A, BT);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);


    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);
    NN_free_tensor_data(f);
    NN_delete_tensor(f);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // matvec
  {
    Tensor *H = NN_rand(2, (size_t[]){N, M}, DTYPE_F32);
    Tensor *HT = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *V = NN_rand(2, (size_t[]){M, 1}, DTYPE_F32);
    Tensor *W = NN_rand(2, (size_t[]){1, M}, DTYPE_F32);
    
    printf("matvec:\t\t");
    Tensor *golden_vec = NN_tensor(2, (size_t[]){N, 1}, DTYPE_F32, NULL);
    Tensor *actual_vec = NN_tensor(2, (size_t[]){N, 1}, DTYPE_F32, NULL);
    Tensor *golden_vect = NN_tensor(2, (size_t[]){1, N}, DTYPE_F32, NULL);
    Tensor *actual_vect = NN_tensor(2, (size_t[]){1, N}, DTYPE_F32, NULL);

    NN_matmul_F32(golden_vec, H, V);
    cycles = READ_CSR("mcycle");
    NN_matmul_F32_RVV(actual_vec, H, V);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden_vec->data, actual_vec->data, N, 1) ? "PASS" : "FAIL", cycles);

    printf("matvec_t:\t");
    NN_transpose_F32(HT, H);
    
    NN_matmul_F32(golden_vect, W, HT);
    cycles = READ_CSR("mcycle");
    NN_matmul_F32_RVV(actual_vect, W, HT);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden_vect->data, actual_vect->data, N, 1) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(H);
    NN_delete_tensor(H);
    NN_free_tensor_data(V);
    NN_delete_tensor(V);
    NN_free_tensor_data(W);
    NN_delete_tensor(W);
  }

  // max and min
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    
    printf("max:\t\t");
    float max_cpu = NN_max_F32(A);
    cycles = READ_CSR("mcycle");
    float max_actual = NN_max_F32_RVV(A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", float_eq(max_cpu, max_actual, 1e-6) ? "PASS" : "FAIL", cycles);

    printf("min:\t\t");
    float min_cpu = NN_min_F32(A);
    cycles = READ_CSR("mcycle");
    float min_actual =  NN_min_F32_RVV(A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", float_eq(min_cpu, min_actual, 1e-6) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
  }

  // matmulf
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *C = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *D = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    
    printf("mulf:\t\t");
    NN_mulF_F32(C, A, 10.0f);
    cycles = READ_CSR("mcycle");
    NN_mulF_F32_RVV(D, A, 10.0f);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(C->data, D->data, M, N) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(C);
    NN_delete_tensor(C);
    NN_free_tensor_data(D);
    NN_delete_tensor(D);
  }

  // matsub
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("matsub:\t\t");
    NN_sub_F32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_sub_F32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // matadd
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("matadd:\t\t");
    NN_add_f32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_add_f32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // matneg
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("cwiseneg:\t");
    NN_neg_F32(golden, A);
    cycles = READ_CSR("mcycle");
    NN_neg_F32_RVV(actual, A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // matcopy
  {

  }

  // cwiseabs
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("cwiseabs:\t");
    NN_abs_f32(golden, A);
    cycles = READ_CSR("mcycle");
    NN_abs_f32_RVV(actual, A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // cwisemin
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("cwiseminimum:\t");
    NN_minimum_F32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_minimum_F32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // cwisemax
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("cwisemaximum:\t");
    NN_maximum_F32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_maximum_F32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // cwisemul
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *golden = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);
    Tensor *actual = NN_tensor(2, (size_t[]){M, N}, DTYPE_F32, NULL);

    printf("matadd:\t\t");
    NN_mul_F32(golden, A, B);
    cycles = READ_CSR("mcycle");
    NN_mul_F32_RVV(actual, A, B);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(golden->data, actual->data, N, M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);

    NN_free_tensor_data(golden);
    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

  // matset
  {

  }

  // matsetv
  {

  }

  // matnorm
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    printf("matnorm:\t");
    float norm_cpu = NN_matrixNorm_F32(A);
    cycles = READ_CSR("mcycle");
    float norm_actual = NN_matrixNorm_F32_RVV(A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", float_eq(norm_cpu, norm_actual, 1e-6) ? "PASS" : "FAIL", cycles);
  }

  // transpose
  {
    Tensor *A = NN_rand(2, (size_t[]){M, N}, DTYPE_F32);
    Tensor *B = NN_tensor(2, (size_t[]){N, M}, DTYPE_F32, NULL);

    printf("transpose:\t");
    
    NN_transpose_F32(B, A);
    cycles = READ_CSR("mcycle");
    NN_transpose_F32(B, A);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", (B->shape[0] == N && B->shape[1] == M) ? "PASS" : "FAIL", cycles);

    NN_free_tensor_data(A);
    NN_delete_tensor(A);
    NN_free_tensor_data(B);
    NN_delete_tensor(B);
  }


  // linear
  {
    int batch = 1;
    int in_features = 3;
    int out_features = 4;

    float weights[] = {-0.00432252, 0.30971584, -0.47518533, -0.4248946,  -0.22236897,  0.15482073, -0.01143914,  0.45777494, -0.0512364,   0.15277413, -0.1744828,  -0.11348708};
    float bias[] = {-0.5515707,  -0.38236874, -0.23799711,  0.02138712};


    Tensor *x = NN_ones(2, (size_t[]){batch, in_features}, DTYPE_F32);
    Tensor *w = NN_tensor(2, (size_t[]){out_features, in_features}, DTYPE_F32, weights);
    Tensor *b = NN_tensor(2, (size_t[]){1, out_features}, DTYPE_F32, bias);

    Tensor *y_golden = NN_tensor(2, (size_t[]){batch, out_features}, DTYPE_F32, NULL);
    Tensor *y_actual = NN_tensor(2, (size_t[]){batch, out_features}, DTYPE_F32, NULL);

    printf("linear:\t\t");

    NN_Linear_F32(y_golden, x, w, b);
    cycles = READ_CSR("mcycle");
    NN_Linear_F32_RVV(y_actual, x, w, b);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(y_golden->data, y_actual->data, out_features, batch) ? "PASS" : "FAIL", cycles);

    printf("weights:\n");
    NN_printf(w);
    printf("bias:\n");
    NN_printf(b);
    printf("input:\n");
    NN_printf(x);
    printf("output:\n");
    NN_printf(y_golden);
  }

  // conv2d
  {
    int batch = 1;
    int in_channels = 1;
    int out_channels = 1;
    int in_height = 3;
    int in_width = 3;
    int kernel_height = 2;
    int kernel_width = 2;
    int stride_height = 1;
    int stride_width = 1;
    int padding_height = 0;
    int padding_width = 0;

    float weights[] = {0.0, 1.0, 2.0, 3.0};
    float bias[] = {0.0};

    Tensor *x = NN_ones(4, (size_t[]){batch, in_channels, in_height, in_width}, DTYPE_F32);
    Tensor *w = NN_tensor(4, (size_t[]){out_channels, in_channels, kernel_height, kernel_width}, DTYPE_F32, weights);
    Tensor *b = NN_tensor(1, (size_t[]){out_channels}, DTYPE_F32, bias);

    Tensor *y_golden = NN_tensor(4, (size_t[]){batch, out_channels, in_height - kernel_height + 1, in_width - kernel_width + 1}, DTYPE_F32, NULL);
    Tensor *y_actual = NN_tensor(4, (size_t[]){batch, out_channels, in_height - kernel_height + 1, in_width - kernel_width + 1}, DTYPE_F32, NULL);

    printf("conv2d:\t\t");

    NN_conv2d_F32(y_golden, x, w, b, (size_t[]){stride_height, stride_width}, (size_t[]){padding_height, padding_width}, 1);
    cycles = READ_CSR("mcycle");
    NN_conv2d_F32_RVV(y_actual, x, w, b, (size_t[]){stride_height, stride_width}, (size_t[]){padding_height, padding_width}, 1);
    cycles = READ_CSR("mcycle") - cycles;
    printf("%s (%lu)\n", compare_2d(y_golden->data, y_actual->data, in_width - kernel_width + 1, in_height - kernel_height + 1) ? "PASS" : "FAIL", cycles);

    printf("weights:\n");
    NN_printf(w);
    printf("bias:\n");
    NN_printf(b);
    printf("input:\n");
    NN_printf(x);
    printf("output:\n");
    NN_printf(y_golden);

    NN_free_tensor_data(x);
    NN_delete_tensor(x);
    NN_free_tensor_data(w);
    NN_delete_tensor(w);
    NN_free_tensor_data(b);
    NN_delete_tensor(b);

  }



  // sum
  // {
  //   Tensor *A = NN_ones(2, (size_t[]){M, N}, DTYPE_F32);
  //   Tensor *B = NN_ones(2, (size_t[]){M, 1}, DTYPE_F32);

  //   ((float *)A->data)[0] = 0;
  //   ((float *)A->data)[1] = 1;
  //   ((float *)A->data)[2] = 2;
  //   ((float *)A->data)[3] = 3;
  //   ((float *)A->data)[4] = 4;
  //   ((float *)A->data)[5] = 5;

  //   printf("shape: (%d, %d)\n", A->shape[0], A->shape[1]);
  //   printf("strides: (%d, %d)\n", A->strides[0], A->strides[1]);
    
  //   printf("shape: (%d, %d)\n", B->shape[0], B->shape[1]);
  //   printf("strides: (%d, %d)\n", B->strides[0], B->strides[1]);



  //   // NN_printf(A);

  //   // transpose
  //   size_t shape_0 = A->shape[0];
  //   size_t shape_1 = A->shape[1];
  //   size_t strides_0 = A->strides[0];
  //   size_t strides_1 = A->strides[1];
  //   A->shape[0] = shape_1;
  //   A->shape[1] = shape_0;
  //   A->strides[0] = strides_1;
  //   A->strides[1] = strides_0;
    
  //   printf("shape: (%d, %d)\n", A->shape[0], A->shape[1]);
  //   printf("strides: (%d, %d)\n", A->strides[0], A->strides[1]);

  //   // NN_printf(A);

  //   printf("sum:\t\t");
  //   float sum_cpu = NN_sum_F32(A);
  //   NN_print_f32(sum_cpu, 4);
  //   printf("\n");
    
  // }



  return 0;
}