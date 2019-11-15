#pragma once

#define SCOMP(i) (*((volatile int *) (0x1000400 + (i) * 4)))

void halt();
