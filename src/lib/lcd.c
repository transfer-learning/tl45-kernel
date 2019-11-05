//
// Created by Will Gulian on 11/3/19.
//

#include "hardware.h"
#include "util.h"

int lcd_util[] = {0, 0, 0, 0};

void __attribute__((noinline)) lcd_putc(char c) {
  static char lol[2][16];
  static int w = 0;
  static int h = 0;
  static int force_newline = 0;

  if (c == '\0') {
    for (int i = 0; i < 16; i++) {
      lol[0][i] = ' ';
      lol[1][i] = ' ';
    }
    w = 0;
    h = 0;
    force_newline = 0;

    LCD1 = 0b001 << 7;
    sad_wait(10);
    return;
  }

  if (w >= 16 || force_newline) {
    force_newline = 0;
    w = 0;
    h++;
    if (h >= 2) {
      for (int i = 0; i < 16; i++) {
        lol[0][i] = lol[1][i];
      }

      // rewrite top line
      LCD1 = 0b001 << 7;
      sad_wait(4);
      for (int i = 0; i < 16; i++) {
        LCD2 = lol[0][i];
        sad_wait(4);
      }

      LCD1 = (0b001 << 7) + 40;
      sad_wait(4);

      for (int i = 0; i < 16; i++) {
        lol[1][i] = ' ';
        LCD2 = ' ';
        sad_wait(4);
      }

      LCD1 = (0b001 << 7) + 40;
      sad_wait(4);

      h = 1;
    } else {
      LCD1 = (0b001 << 7) + 40;
      sad_wait(4);
    }
  }

  if (c == '\n') {
    force_newline = 1;
    return;
  }

  LCD2 = c;
  lol[h][w++] = c;
  sad_wait(4);
}

void lcd_puts(const char *str) {
  char c;
  while ((c = *(str++)) != '\0') {
    lcd_putc(c);
  }
}

void lcd_flash(const char *str) {
  lcd_puts(str);
  sad_wait(1000);
  lcd_putc('\n');
}

void lcd_reset() {
  LCD1 = 0x3c;
  sad_wait(10);
  LCD1 = 0x3c;
  sad_wait(10);

  LCD1 = 0x3c;
  sad_wait(10);

  LCD1 = 0x3c;
  sad_wait(10);
  LCD1 = 0x0c;
  sad_wait(10);
  LCD1 = 0x01;
  sad_wait(10);
  LCD1 = 0x06;
  sad_wait(10);

  // rewrite top line
  LCD1 = 0b001 << 7;
  sad_wait(10);

  // reset terminal
  lcd_putc('\0');
}


// TODO this function cannot be called, causes decode_err
int pref_lcd(int a, int b) {
  return a + (b ^ a);
}
