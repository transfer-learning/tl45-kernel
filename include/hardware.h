//
// Created by Will Gulian on 11/3/19.
//

#ifndef SIM_TL45_HARDWARE_H
#define SIM_TL45_HARDWARE_H

#define SEG (*((volatile int *) 0x1000000))
#define SWITCHES (*((volatile int *) 0x1000004))

#define LCD1 (*((volatile int *) 0x1000008))
#define LCD2 (*((volatile int *) 0x100000c))

#define SD_CMD (*((volatile int *) 0x1000020))
#define SD_DATA (*((volatile int *) 0x1000024))
#define SD_FIFO_A (*((volatile int *) 0x1000028))
#define SD_FIFO_B (*((volatile int *) 0x100002c))

#define L_ENCODER (*((volatile int *) 0x01000020))
#define R_ENCODER (*((volatile int *) 0x01000024))

#define TIMER (*((volatile int *) 0x01000028))

#define VER(i) (*((volatile int *) (0x13fc000 + (i) * 4)))
#define VER0 VER(0)
#define VER1 VER(1)

#endif //SIM_TL45_HARDWARE_H
