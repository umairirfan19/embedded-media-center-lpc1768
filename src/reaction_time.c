#include <LPC17xx.H>
#include "GLCD.h"
#include "KBD.h"
#include "LED.h"
#include <stdbool.h>
#include <stdlib.h>  // For random number generation
#include <stdio.h>   // For sprintf

#define JOY_CENTER (1 << 0) // Joystick center

// Function Prototypes
void delay_ms(uint32_t ms);
uint32_t get_random_delay(void);

// Main function
int reaction_time_main(void) {
    uint32_t start_time, reaction_time;
    uint32_t delay_time;
    bool button_pressed = false;

    // Initialize peripherals
    GLCD_Init();
    KBD_Init();
    LED_Init();

    GLCD_Clear(White);
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Blue);

    while (1) {
        // 1. Display "Get Ready" message
        GLCD_Clear(White);
        GLCD_DisplayString(3, 0, 1, (unsigned char *)"Get Ready...");
        LED_On(0);  // Turn on LED for effect
        delay_ms(1000); // Wait for 1 second
        LED_Off(0); // Turn off LED

        // 2. Generate a random delay
        delay_time = get_random_delay();
        delay_ms(delay_time);

        // 3. Display "Press!" message
        GLCD_Clear(White);
        GLCD_DisplayString(3, 0, 1, (unsigned char *)"Press!");

        // 4. Record the start time and wait for button press
        start_time = SysTick->VAL; // Get the current tick count
        button_pressed = false;
        while (!button_pressed) {
            // Check if the joystick center button is pressed
            if (KBD_GetKeys() & JOY_CENTER) {
                button_pressed = true;
                reaction_time = start_time - SysTick->VAL; // Calculate reaction time
                reaction_time = reaction_time / (SystemCoreClock / 1000); // Convert to ms
            }
        }

        // 5. Display the reaction time
        char buffer[16];
        sprintf(buffer, "Time: %lu ms", reaction_time);
        GLCD_Clear(White);
        GLCD_DisplayString(3, 0, 1, (unsigned char *)buffer);

        // 6. Wait before restarting the game
        delay_ms(2000);
    }
}

// Function: Generate a random delay (1-3 seconds)
uint32_t get_random_delay(void) {
    return 1000 + (rand() % 2000); // Random delay between 1 and 3 seconds
}

// Function: Simple delay in milliseconds
void delay_ms(uint32_t ms) {
    uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 5000; j++) __NOP();
}
