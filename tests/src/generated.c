
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rv.h>

#include "nn.h"
#include "unittest.h"

int main() {
  enable_accelerator_features();

  size_t cycles = 0;

  
  {
    printf("rms_norm:               ");

    // [-0.0374341   2.682218   -4.115226   -3.6796951  -1.9257718   1.3407868  -0.0990
    Tensor *x = NN_tensor(1, (size_t[]){ 69 }, DTYPE_F32, (uint8_t[]){ 0x80,0x54,0x19,0xbd,0x76,0xa9,0x2b,0x40,0xee,0xaf,0x83,0xc0,0x20,0x80,0x6b,0xc0,0xb1,0x7f,0xf6,0xbf,0xe7,0x9e,0xab,0x3f,0x10,0xe3,0xca,0xbd,0x82,0xb9,0x7d,0x40,0x4c,0x2f,0xe3,0xbe,0x1f,0x5a,0xa9,0x3f,0x97,0x6a,0xc1,0xbf,0x8c,0x9a,0x7b,0xbf,0x13,0xdb,0x98,0xc0,0x26,0xee,0x53,0xc0,0x51,0xe9,0x3,0xc0,0xc0,0xa9,0x3d,0x3e,0xb8,0x3,0xfd,0x3f,0xde,0x1,0x40,0x40,0xef,0xf0,0x58,0xc0,0x1e,0x59,0xb,0xc0,0x7e,0x75,0xe8,0x3f,0xb1,0xdc,0x84,0x40,0x4d,0xb6,0x83,0xbf,0xb3,0x75,0x6f,0x40,0x90,0x50,0x4e,0xbf,0x2a,0x71,0x7,0x3f,0x4e,0xe0,0x90,0x40,0x61,0x6d,0x94,0xc0,0xc0,0x73,0x49,0xc0,0x98,0x6,0xa2,0xbf,0xd3,0x78,0xf9,0xbf,0x79,0x3d,0x8a,0x40,0xe0,0x6a,0x4f,0xc0,0x78,0x4e,0x13,0xc0,0xa0,0x90,0x5f,0xc0,0x8a,0xd9,0x95,0xc0,0x5,0xcc,0x3a,0xc0,0x23,0x89,0x89,0x40,0x36,0xca,0xe,0x40,0x60,0x18,0x1b,0x40,0x68,0xa2,0x86,0x3e,0x8,0xf,0x24,0xc0,0x70,0x8e,0x58,0x3f,0x23,0x64,0x95,0xc0,0xa0,0x38,0x67,0xc0,0x38,0xf8,0x24,0xc0,0x70,0xe6,0x49,0x40,0x70,0x9f,0x3b,0x40,0x1d,0xeb,0xd,0xc0,0xe8,0xbd,0x38,0xbe,0xd0,0xa8,0x4c,0x40,0xb1,0xf,0x9f,0x40,0x2d,0x1,0xfe,0x3f,0x3a,0xeb,0x2c,0x3f,0x3e,0x8e,0x56,0x40,0xb0,0x6a,0x3c,0xc0,0x36,0x85,0x6e,0x3f,0x7,0x19,0x78,0xc0,0x9e,0xc9,0x5d,0xc0,0x86,0x4e,0x25,0xc0,0x98,0xca,0x10,0x40,0xfb,0xb0,0x0,0x40,0x84,0x8d,0x3d,0xc0,0x3a,0x59,0xc1,0x3f,0xca,0xab,0x2f,0x40,0xe8,0x8e,0x21,0xbf,0x50,0x7d,0x43,0x3e,0x80,0x4a,0x94,0x3f,0x40,0x85,0x46,0x40 });
    // [ 4.8009706  -3.853118   -1.8323487   1.9650495   4.142747    4.3510365   4.4117
    Tensor *w = NN_tensor(1, (size_t[]){ 69 }, DTYPE_F32, (uint8_t[]){ 0x8d,0xa1,0x99,0x40,0x7c,0x99,0x76,0xc0,0x67,0x8a,0xea,0xbf,0xbe,0x86,0xfb,0x3f,0x62,0x91,0x84,0x40,0xb1,0x3b,0x8b,0x40,0x55,0x2d,0x8d,0x40,0x16,0xbd,0x7e,0x3f,0x1b,0x22,0x8b,0xc0,0x34,0x80,0xeb,0x3e,0x97,0x31,0x48,0xc0,0xd8,0x1c,0x95,0xc0,0xa7,0x28,0x8e,0x40,0xac,0x50,0x73,0x40,0xbf,0x9a,0x9f,0xc0,0x8a,0x94,0x6f,0x3f,0xfa,0xa0,0x57,0xbf,0x66,0xa3,0x52,0xbf,0x72,0x7b,0x12,0xc0,0xaf,0x1d,0xf6,0x3f,0x81,0x89,0x3d,0xc0,0x53,0x9e,0xea,0x3f,0x9c,0xd3,0x21,0x40,0x33,0x14,0x65,0x40,0x9a,0x4d,0xef,0x3f,0x8e,0x5b,0x9e,0xc0,0x40,0x95,0x4f,0xc0,0xe3,0xc7,0x1f,0x40,0xef,0xf3,0x85,0x3f,0x7c,0xa0,0x79,0xc0,0x22,0x43,0x38,0xc0,0x17,0x85,0x96,0x40,0x29,0x9f,0x57,0x40,0x2e,0x87,0xb,0xc0,0x9,0x14,0xa1,0xbf,0x6b,0x6a,0x98,0xc0,0x20,0xe,0xb8,0xbd,0x96,0xfa,0x70,0xc0,0x8a,0xd5,0x76,0xc0,0x14,0xe,0x8d,0xbe,0x88,0x2f,0x40,0x3f,0xb8,0xc,0x3,0xc0,0x7e,0xe1,0x3d,0x40,0x86,0xbb,0x42,0xc0,0xe1,0x2d,0x91,0x40,0xc4,0x4b,0x5b,0x40,0xde,0xec,0x86,0xc0,0x35,0x49,0x9f,0xbf,0x20,0x7,0x67,0x3e,0xde,0xc0,0x3a,0x3f,0xa2,0xca,0x97,0x3f,0x73,0x27,0xfb,0x3f,0x2c,0x58,0x99,0x3e,0x20,0x23,0x1c,0xc0,0xa4,0x6b,0x17,0x40,0xd6,0x7a,0x99,0xc0,0x88,0xaa,0x3d,0xc0,0xb,0x36,0xa0,0xbf,0x54,0xe0,0x1b,0xc0,0xb3,0xe4,0xdf,0xbf,0xb4,0x23,0x83,0xc0,0x40,0x23,0x88,0xbf,0xdb,0xcd,0x88,0x3f,0x14,0x78,0x50,0xc0,0xa8,0x60,0x83,0xbe,0x80,0x12,0x65,0x40,0x96,0x95,0x3,0xbf,0xd0,0x4b,0xe,0x3e,0x30,0xd9,0xdc,0xbe });


    // [-0.06442871 -3.7050097   2.7032409  -2.5921988  -2.860067    2.0913918  -0.1566
    Tensor *golden = NN_tensor(1, (size_t[]){ 69 }, DTYPE_F32, (uint8_t[]){ 0x33,0xf3,0x83,0xbd,0xe1,0x1e,0x6d,0xc0,0xe6,0x1,0x2d,0x40,0x96,0xe6,0x25,0xc0,0x56,0xb,0x37,0xc0,0x5d,0xd9,0x5,0x40,0x76,0x71,0x20,0xbe,0x88,0x5,0xb5,0x3f,0x95,0xe,0x31,0x3f,0xbe,0x66,0x5f,0x3e,0xbf,0xe4,0xd8,0x3f,0xa,0x27,0xd2,0x3f,0x27,0x70,0xf3,0xc0,0x2e,0x6c,0x90,0xc0,0x0,0xdd,0x6b,0x40,0x5a,0x87,0x7e,0x3d,0xf9,0xcc,0x18,0xbf,0xfe,0x8b,0x62,0xbf,0x7,0x1,0x32,0x40,0x77,0x1b,0xc0,0xbf,0x91,0xcc,0xf6,0xbf,0xdc,0x9b,0x2e,0x40,0x2e,0xc9,0x6e,0xbf,0x9a,0xa2,0x99,0x40,0xa,0x47,0xa,0xbf,0xcb,0x48,0x70,0xbf,0x43,0x75,0xa8,0xc0,0x3,0xd8,0x84,0xc0,0x12,0x28,0x97,0xbf,0xb7,0x8e,0xe2,0x3f,0xc6,0xbe,0x0,0x40,0x2b,0x1c,0xe9,0x40,0xe3,0x84,0x7a,0xc0,0x48,0x42,0xe6,0x3f,0xb0,0xb7,0xc9,0x3f,0x9f,0xde,0xff,0x40,0x93,0x95,0xc0,0x3d,0xa9,0xa6,0xb9,0xc0,0x35,0x6d,0x45,0xc0,0x36,0x16,0x75,0xbe,0xf0,0xef,0x90,0x3d,0xab,0xdc,0xf0,0x3f,0x1c,0x55,0x66,0x3f,0x5a,0xf4,0xa2,0x40,0x8e,0x8,0xbc,0xc0,0x51,0xa5,0x4a,0xc0,0xa7,0x97,0x98,0xc0,0x73,0x67,0xa7,0xbf,0x8,0xa8,0x37,0xbe,0xf5,0x41,0x41,0xbd,0x63,0x3,0xae,0x3f,0xe6,0xc5,0x5f,0x40,0xc6,0x2d,0x5a,0x3e,0x1c,0x3c,0x17,0xbf,0x4b,0xfb,0x35,0x40,0x10,0xfc,0xa1,0x40,0x22,0x68,0x7d,0xbf,0xd8,0xa5,0xde,0x3f,0xb5,0xa6,0x41,0x40,0x21,0x51,0xcf,0x3f,0x6a,0xb8,0x54,0xc0,0xe6,0x45,0x44,0xbf,0x63,0x41,0x91,0xbf,0xae,0xc7,0xe1,0xbf,0x26,0x47,0x81,0xbe,0x72,0x4d,0x4f,0xbf,0xc3,0x16,0x10,0xbd,0x7d,0x65,0x6c,0x3d,0xdf,0x95,0xf5,0xbe });
    Tensor *actual = NN_zeros(1, (size_t[]){ 69 }, DTYPE_F32);

    cycles = read_cycles();
    NN_rms_norm(actual, x, w, 1e-6);                          
    cycles = read_cycles() - cycles;
    printf("%s  (%lu cycles)\n", compare_tensor(golden, actual, 1e-4) ? "PASS" : "FAIL", cycles);

    NN_delete_tensor(x);
    NN_delete_tensor(w);

    NN_delete_tensor(golden);
    NN_free_tensor_data(actual);
    NN_delete_tensor(actual);
  }

}