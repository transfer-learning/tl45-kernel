//
// Created by Will Gulian on 11/15/19.
//

#include "verilator.h"
#include "fxp.h"

int main() {

  while(1) {

    // The i argument for RECV() and PRNT() do not do anything. It does not correspond to the argument count.
    // To read multiple arguments, call RECV() multiple times.

    fxp_t a = (fxp_t) RECV(0);
    fxp_t b = (fxp_t) RECV(0);

    fxp_t c = fxp_div(a, b);

    // fxp_t c = fxp_mul(a, b);

    PRNT(0, c);
  }

}


// TODO this function cannot be called, causes decode_err
int pref_main(int a, int b) {
  return a + (b ^ a);
}
