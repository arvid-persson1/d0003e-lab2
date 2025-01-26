#include "tinythreads.h"
#include "lab1.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>

int pp;
Mutex m;

void printAt(long num, int pos) {
    lock(&m);
    pp = pos;
    writeChar(num % 100 / 10 + '0', pp++);
    writeChar(num % 10 + '0', pp);
    unlock(&m);
}

void computePrimes(int pos) {
    long n = 1;

    while (true) {
        if (isPrime(n))
            printAt(n, pos);
        n++;
    }
}

int main() {
    initClk();
    initLcd();

    spawn(computePrimes, 0);
    computePrimes(3);
}
