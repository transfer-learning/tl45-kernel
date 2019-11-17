//
// Created by Will Gulian on 11/3/19.
//

#define SEG (*((volatile int *) 0x1000000))
#define SWITCHES (*((volatile int *) 0x1000004))


#define VER(i) (*((volatile int *) (0x13fc000 + (i) * 4)))
#define VER0 VER(0)
#define VER1 VER(1)

#define SCOMP(i) (*((volatile int *) (0x1000400 + (i) * 4)))

void halt();

//    0000 0001 0000 0000 0000 01xx xxxx xx - SCOMP
//    0000 0001 0000 0000 0000 0100 0000 0000

__attribute__((noinline))
void func(int a) {
  if (a != 2) {
    SEG = a;
  } else {
    SEG = 0;
  }
}


void main() {

//  SCOMP(0) = 0xdeadbeef;
//  SCOMP(1) = 0xb0bacafe;
//
//  SCOMP(2);
//  SCOMP(3);

while (1) {

 // SEG = (SWITCHES & 0xFF) * ((SWITCHES >> 8) & 0xFF);

  func(SWITCHES);
}

  halt();
}

int perf_unused(int a, int b) {
  return a + (a ^ b);
}
