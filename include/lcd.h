//
// Created by Will Gulian on 11/3/19.
//

#ifndef SIM_TL45_LCD_H
#define SIM_TL45_LCD_H

void lcd_putc(char c);
void lcd_puts(const char *str);
void lcd_flash(const char *str);
void lcd_reset();

#endif //SIM_TL45_LCD_H
