#include "hardware.h"
#include "lcd.h"
#include "util.h"


unsigned int fib(unsigned int n) {
  if (n < 2) return 1;
  return fib(n-1) + fib(n-2);
}

int main() {
  lcd_reset();
  char buf[20];
  for (unsigned int i = 0; i < 32; i++)
  {
    unsigned int fibNum = fib(i);
    sprint_int(buf, fibNum);
    lcd_puts(buf);
    lcd_putc('\n');
  }
  

  return 0;
}
