#ifndef _LAB1_H
#define _LAB1_H

#include <stdbool.h>

void initClk(void);
void initLcd(void);
void initButton(void);

void clearChar(int pos);
void writeChar(char ch, int pos);

bool isPrime(unsigned long n);

#endif
