#include "MKL25Z4.h"
#include <stdlib.h>

#define ZERO 0xFFFFFB86
#define ONE 0xFFFFFFDE
#define TWO 0xFFFFF787
#define THREE 0xFFFFF796
#define FOUR 0xFFFFF3DE
#define FIVE 0xFFFFF3B6
#define SIX 0xFFFFF3A6
#define SEVEN 0xFFFFFF9E
#define EIGHT 0xFFFFF386
#define NINE 0xFFFFF396

uint32_t li[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};

int main(void) {
    int i, j, k;
    int digits[7] = {0}; 
    int intensity = 0xFF; 
    
    SIM_SCGC5 |= 0x0800;
    GPIOC_PDDR = 0xFFFFFFFF;
    PORTC_PCR3 = 0x00000100;
    PORTC_PCR4 = 0x00000100;
    PORTC_PCR5 = 0x00000100;
    PORTC_PCR6 = 0x00000100;
    PORTC_PCR11 = 0x00000100;
    PORTC_PCR10 = 0x00000100;
    PORTC_PCR0 = 0x00000100;
    PORTC_PCR7 = 0x00000100;
    
    SIM_SCGC5 |= 0x0400;
    PORTB_PCR1 = 0x00000100;
    PORTB_PCR2 = 0x00000100;
    GPIOB_PDDR = 0xFFFFFFFF;
    
    while (1) {
        for (i = 0; i < 10000000; i++) { 

            int temp = i;
            for (int d = 6; d >= 0; d--) {
                digits[d] = temp % 10;
                temp /= 10;
            }
            

            for (int fade = 0; fade < 255; fade++) {
                intensity = fade;
                for (int d = 0; d < 7; d++) {
                    for (j = 0; j < 5000; j++) {
                        GPIOB_PDOR = 0x00000002 << d; 
                        GPIOC_PDOR = li[digits[d]]; 
                        for (k = 0; k < intensity; k++) { 
                            __NOP();
                        }
                    }
                }
            }
        }
    }
}
