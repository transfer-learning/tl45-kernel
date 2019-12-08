#include "hardware.h"
#include "lcd.h"
#include "util.h"
#include "md2.h"
#include "string.h"

#define F(x) do{ *((volatile int*)(0x0)) = (x); } while(0);
int main ()
{
  memset(0, 255,255);
  return 0;
}
