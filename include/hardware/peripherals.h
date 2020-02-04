#ifndef SIM_TL45_HARDWARE_H
#define SIM_TL45_HARDWARE_H

#define SEG (*((volatile int *) 0x1000000))
#define SWITCHES (*((volatile int *) 0x1000004))

#define LCD1 (*((volatile int *) 0x1000008))
#define LCD2 (*((volatile int *) 0x100000c))

#define TIMERL (*((volatile int *) 0xFFFFFFF0))
#define TIMERH (*((volatile int *) 0xFFFFFFF4))

#define UART_CTRL (*((volatile int *) 0xFFFFFFF8))
#define UART (*((volatile unsigned char*) 0xFFFFFFFF))

#endif //SIM_TL45_HARDWARE_H
