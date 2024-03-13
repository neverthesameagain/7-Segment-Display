//importing necessary libraries
#include "MKL25Z4.h"
#include <stdlib.h>

#define ZERO  0xFFFFFB86
#define ONE   0xFFFFFFDE
#define TWO   0xFFFFF787
#define THREE 0xFFFFF796
#define FOUR  0xFFFFF3DE
#define FIVE  0xFFFFF3B6
#define SIX   0xFFFFF3A6
#define SEVEN 0xFFFFFF9E
#define EIGHT 0xFFFFF386
#define NINE  0xFFFFF396
#define b 0xFFFFF3E6
#define A 0xFFFFF38E
#define t 0xFFFFF3E7
#define C 0xFFFFFBA7
#define h 0xFFFFF3EE
#define SPACE 0XFFFFFFFF

//initialising PWM TPMO for each serial
void init_pwm() {
    //Enabling Clock source, here the bus clock
    SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); // Assuming bus clock as the source

    // Configuring TPM0 for center-aligned PWM mode
    TPM0_SC = TPM_SC_PS(0); 
    TPM0_SC |= TPM_SC_CMOD(1); // Enabling TPM counter
    TPM0_SC |= TPM_SC_CPWMS_MASK; // Center-aligned PWM mode
    TPM0_SC &= ~TPM_SC_TOIE_MASK; // Disabling overflow interrupts

    // Configuring TPM0 channel 0 (PTC1) for PWM output
    TPM0_C0SC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; 
    TPM0_C0V = 0; //initialise with 0 intensity
}

int main(void) {
    int i = 0;
    int j, k;
    
    // Initialize PWM for intensity control
    init_pwm();

    // Activating the serials and ports of the microprocessor
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
    PORTB_PCR8 = 0x00000100;
    PORTB_PCR9 = 0x00000100;
    PORTB_PCR10 = 0x00000100;
    PORTB_PCR11 = 0x00000100;
    PORTB_PCR3 = 0x00000100;
    GPIOB_PDDR = 0xFFFFFFFF;

    // Establishing a list to call out values based on the pattern
    uint32_t li[] = {b, A, t, C, h, SPACE, TWO, ZERO, TWO, TWO, SPACE};

    while (1) {
        // Gradually decrease PWM duty cycle to fade out the displays
        for (int duty_cycle = 100; duty_cycle >= 0; duty_cycle--) {
            TPM0_C0V = (duty_cycle * TPM0_MOD) / 100; // Update duty cycle

            for (i = 0; i < 100000; i++) {
                for (j = 0; j < 1000; j++) {
                    //for display 1 corresponding to unit digit display, here i manually defined for each display and did not rmake a genral loop because the pins i used for enabling the displays were not in a continous order.
                    GPIOB_PDOR = 0x00000002;\
                    //telling which combination to turn on for this display
                    GPIOC_PDOR = li[i];
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    
                    GPIOB_PDOR = 0x00000004;\
                    GPIOC_PDOR = li[(i + 6) % 10]; 
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    //defining display number using left space , by the hexadecimal equivalence, here in 1<<q, q is the B port number.
                    GPIOB_PDOR = 1<<3;
                    GPIOC_PDOR = li[(i + 4) % 10]; \
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    //similarly done for all 7 digits
                    GPIOB_PDOR = 1<<11;
                    GPIOC_PDOR = li[(i + 3) % 10]; \
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    GPIOB_PDOR = 1<<10;
                    GPIOC_PDOR = li[(i + 2) % 10]; \
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    GPIOB_PDOR = 1<<9;
                    GPIOC_PDOR = li[(i + 1) % 10]; \
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    GPIOB_PDOR = 1<<8;
                    GPIOC_PDOR = li[(i) % 10]; \
                    for (k = 0; k < 500; k++) {
                        __NOP();
                    }
                    
                }
            }
            }
        }
    }

