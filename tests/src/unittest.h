
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <rv.h>

#include "nn.h"

#ifdef X86
  #include <immintrin.h>
  #include <x86intrin.h>
#endif

#ifdef RVV
  #include "riscv_vector.h"
#endif

static void enable_accelerator_features() {
  #ifdef RVV
    // enable vector operation
    unsigned long mstatus;
    asm volatile("csrr %0, mstatus" : "=r"(mstatus));
    mstatus |= 0x00000400 | 0x00004000 | 0x00010000;
    asm volatile("csrw mstatus, %0"::"r"(mstatus));
  #endif
}

static size_t read_cycles() {
  #ifdef X86
    return __rdtsc();
  #elif defined(RISCV)
    return READ_CSR("mcycle");
  #endif
}
