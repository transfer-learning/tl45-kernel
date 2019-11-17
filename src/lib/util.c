//
// Created by Will Gulian on 11/3/19.
//

volatile unsigned int g;

void sad_wait(int ms) {
  for (int i = 0; i < ms; i++) {
    for (int j = 0; j < 1000; j++) {
      g = j;
    }
  }
}

unsigned __udivsi3(unsigned n, unsigned d);

int sprint_int(char *arr, int num) {
  char foo[20];
  int i = 0;

  unsigned int my_num = (unsigned) num;

  do {
    unsigned new = my_num >> 4;//;__udivsi3(my_num, 10u);
    unsigned m = my_num - (new << 4);

    foo[i++] = (m < 10 ? '0' : 'A' - 10) + m;
    my_num = new;
  } while (my_num != 0);
  int n = i;

  while (i-- > 0) {
    *arr++ = foo[i];
  }
  *arr = 0;

  return n;
}


void *memcpy(void *dst, const void *src, int n) {
  void *orig_dst = dst;

  for (int i = 0; i < n; i++) {
    ((char *) dst)[i] = ((char *) src)[i];
  }

  return orig_dst;
}


void *memset(void *dst, char c, int n) {
  void *orig_dst = dst;

  for (int i = 0; i < n; i++) {
    ((char *) dst)[i] = c;
  }

  return orig_dst;
}

// TODO this function cannot be called, causes decode_err
int pref_utilf(int a, int b) {
  return a + (b ^ a);
}