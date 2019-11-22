#include "hardware.h"
#include "lcd.h"
#include "util.h"
#include "md2.h"
#include "string.h"

  char text2[] = "aa";
  unsigned char hash[20];

int main() {
  lcd_reset();
  wait(500);

  char buf[10];
  sprint_int(buf, strlen(text2));
  lcd_puts(buf);
  lcd_putc('\n');
  lcd_puts(text2);

  MD2_CTX ctx;
  md2_init(&ctx);
  md2_update(&ctx, text2, strlen(text2));
  md2_final(&ctx, hash);
  for(;;) {
    SEG = hash;
  }

  for(;;) {
    SEG = 0xdeadbeef;
    wait(1000);
    SEG = 0xb0bacafe;
    wait(1000);
  }

  return 0;
}
