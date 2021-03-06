// RUN: %cladclang %s -I%S/../../include -oBasicArithmeticAddSub.out 2>&1 | FileCheck %s
// RUN: ./BasicArithmeticAddSub.out | FileCheck -check-prefix=CHECK-EXEC %s

//CHECK-NOT: {{.*error|warning|note:.*}}

#include "clad/Differentiator/Differentiator.h"

extern "C" int printf(const char* fmt, ...);

int f_pow(int arg, int p) {
  if (p == 0)
    return 1;
  else
    return arg * f_pow(arg, p - 1);
}

// CHECK: int f_pow_darg0(int arg, int p) {
// CHECK-NEXT:   int _d_arg = 1;
// CHECK-NEXT:   int _d_p = 0;
// CHECK-NEXT:   if (p == 0)
// CHECK-NEXT:     return 0;
// CHECK-NEXT:   else {
// CHECK-NEXT:     int _t0 = f_pow(arg, p - 1);
// CHECK-NEXT:     return _d_arg * _t0 + arg * (f_pow_darg0(arg, p - 1) * (_d_arg + _d_p - 0));
// CHECK-NEXT:   }
// CHECK-NEXT: }

int f_pow_darg0(int arg, int p);
// == p * f_pow(arg, p - 1)

int main() {
  clad::differentiate(f_pow, 0);
  printf("Result is = %d\n", f_pow_darg0(10, 2)); // CHECK-EXEC: Result is = 20
}
