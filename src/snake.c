#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Board_Buttons.h"
#include "GLCD.h"
#include "KBD.h"

// Direction macros
#define UP 8
#define DOWN 32
#define LEFT 64
#define RIGHT 16
#define NEUTRAL 0
#define PRESSED 1

// Screen boundaries (Y-rows: 0-9 for text; X-cols: 0-19 for text)
#define SCREEN_LENGTH 9     // Usable rows for game: 1 to 8 (9 total)
#define SCREEN_WIDTH 19     // Usable columns for game: 1 to 18 (19 total)

// Font index 16x24
#define __FI 1

int snake[144][2];
int food_x, food_y;

int snake_x, snake_y;
int snake_length = 3;
// int food_toggle = 0; // Not strictly needed, simplifying food display

// Variable to store the *previous* non-neutral direction
int current_direction = RIGHT;

// Helper function for simple timing delay
void delay() {
    int i;
    // Reduced delay for smoother, faster gameplay
    for (i = 0; i < 1500000; i++) {
        __nop();
        __nop();
    }
}

// Sets up the game borders
void setup_window() {
    int i;

    // Clear the screen area first
    GLCD_Clear(White);
    
    // Set color for the borders (Dark Cyan - a nice color for a game)
    GLCD_SetTextColor(DarkCyan);

    // Draw top and bottom borders (Row 0 and Row 9)
    GLCD_DisplayString(0, 0, __FI,
                       (unsigned char *)"--------------------"); // 20 chars
    GLCD_DisplayString(9, 0, __FI,
                       (unsigned char *)"--------------------"); // 20 chars
    
    // Draw side borders (Col 0 and Col 19)
    for (i = 1; i < SCREEN_LENGTH; i++) {
        GLCD_DisplayString(i, 0, __FI, (unsigned char *)"|");
        GLCD_DisplayString(i, 19, __FI, (unsigned char *)"|");
    }
    
    GLCD_SetTextColor(Black); // Set text color back to black for game elements
}

// Initializes the snake's starting position
void init_game() {
    snake_x = 5; // Start row (Y)
    snake_y = 5; // Start column (X)
    current_direction = RIGHT; // Start moving right

    // Snake head at index 0 (y, x)
    snake[0][0] = snake_x;
    snake[0][1] = snake_y;

    // Initial body segments
    snake[1][0] = snake_x;
    snake[1][1] = snake_y - 1;

    snake[2][0] = snake_x;
    snake[2][1] = snake_y - 2;
    
    snake_length = 3; // Reset length
}

// Helper: Checks if generated food coordinates overlap with the snake's body
int generate_food_helper(int test_x, int test_y) {
    int i;
    // Food coordinates are (y, x) in this GLCD display system
    for (i = 0; i < snake_length; i++) {
        // If food position matches any snake segment position, return 0 (overlap)
        if (test_y == snake[i][0] && test_x == snake[i][1]) {
            return 0;
        }
    }
    return 1; // No overlap, food is valid
}

// Generates food in a random, valid location
void generate_food() {
    // Generate food in the safe area (rows 1-8, columns 1-18)
    do {
        // food_y is the row index (1 to 8, which is SCREEN_LENGTH - 1)
        food_y = (rand() % (SCREEN_LENGTH - 1)) + 1;
        // food_x is the column index (1 to 18, which is SCREEN_WIDTH - 1)
        food_x = (rand() % (SCREEN_WIDTH - 1)) + 1;

    } while (!generate_food_helper(food_x, food_y));

    // Display food immediately after generation
    // Using a clear dot 'O' for better visibility against the white background
    GLCD_SetTextColor(Red);
    GLCD_DisplayChar(food_y, food_x, __FI, 'O');
    GLCD_SetTextColor(Black); // Restore default
}

// Detects self-collision or wall collision
int detect_collisions() {
    int i;
    int head_y = snake[0][0];
    int head_x = snake[0][1];
    
    // 1. Check wall collision (boundaries are 0 and 9 for Y, 0 and 19 for X)
    if (head_y <= 0 || head_y >= SCREEN_LENGTH || head_x <= 0 || head_x >= SCREEN_WIDTH) {
        return 1;
    }

    // 2. Check self-collision (start checking from segment 1, as 0 is the head)
    for (i = 1; i < snake_length; i++) {
        if (head_y == snake[i][0] && head_x == snake[i][1]) {
            return 1;
        }
    }

    return 0;
}

// Updates snake positions, handles head movement, and draws the snake
void update_snake() {
    int i;
    int old_tail_y, old_tail_x;

    // Store the coordinates of the tail segment *before* the shift
    old_tail_y = snake[snake_length - 1][0];
    old_tail_x = snake[snake_length - 1][1];

    // Shift all body segments one position forward (starting from the tail)
    for (i = snake_length - 1; i > 0; i--) {
        snake[i][0] = snake[i - 1][0]; // Y
        snake[i][1] = snake[i - 1][1]; // X
    }

    // Move the head to the new calculated position
    // This makes the movement smoother and atomic
    snake[0][0] = snake_x;
    snake[0][1] = snake_y;

    // 1. Draw the new head (using a filled block character)
    GLCD_SetTextColor(Blue); // Distinct color for the head
    GLCD_DisplayChar(snake[0][0], snake[0][1], __FI, 'H'); // 'H' for Head
    GLCD_SetTextColor(Black); // Restore default

    // 2. Draw the new segment that was the old head (using a body character)
    // Only draw the previous head if the length is > 1
    if (snake_length > 1) {
        GLCD_SetTextColor(DarkGreen); // Color for the body
        GLCD_DisplayChar(snake[1][0], snake[1][1], __FI, '#'); // '#' for Body Segment
        GLCD_SetTextColor(Black); // Restore default
    }

    // 3. Clear the old tail segment, but ONLY if the tail did not land on food
    // If the snake length increased (it ate food), we do NOT clear the tail
    // Since this function runs on every loop, we rely on the game loop logic
    // to update snake_length only when food is eaten.
    
    // If snake_length did NOT change this cycle, clear the old tail segment
    // We check if the tail position matches the food position (only clear if it's NOT food)
    if (old_tail_y != food_y || old_tail_x != food_x) {
        // Clear the old tail position
        GLCD_DisplayChar(old_tail_y, old_tail_x, __FI, ' '); 
    }
}

// Main game loop
int snake_game() {
    int score = 0;
    int input_direction;
    int new_direction;

    // Initialize game state, borders, and snake
    init_game();
    setup_window();
    
    // Generate and display the first piece of food
    generate_food(); 

    // Display score header (Row 10, below the game board)
    GLCD_SetTextColor(Black);
    GLCD_DisplayString(10, 0, __FI, (unsigned char *)"SCORE: 0");

    while (1) {
        // 1. Get User Input
        input_direction = get_button();

        // 2. Determine New Direction (Prevent 180-degree turn)
        new_direction = current_direction;
        
        if (input_direction == UP && current_direction != DOWN) {
            new_direction = UP;
        } else if (input_direction == DOWN && current_direction != UP) {
            new_direction = DOWN;
        } else if (input_direction == LEFT && current_direction != RIGHT) {
            new_direction = LEFT;
        } else if (input_direction == RIGHT && current_direction != LEFT) {
            new_direction = RIGHT;
        }

        // Only update current_direction if a valid, non-reversal input was given
        // This makes the snake move continuously even without constant input
        if (input_direction != NEUTRAL && input_direction != PRESSED) {
            current_direction = new_direction;
        }
        
        // 3. Calculate next head position based on current_direction
        switch (current_direction) {
            case UP:
                snake_x = snake[0][0] - 1; // Y decreases
                snake_y = snake[0][1];
                break;
            case DOWN:
                snake_x = snake[0][0] + 1; // Y increases
                snake_y = snake[0][1];
                break;
            case LEFT:
                snake_x = snake[0][0];
                snake_y = snake[0][1] - 1; // X decreases
                break;
            case RIGHT:
                snake_x = snake[0][0];
                snake_y = snake[0][1] + 1; // X increases
                break;
            default:
                // Should not happen if initialized correctly, but ensures continuous movement
                break;
        }
        
        // 4. Update the snake's body and display
        update_snake();

        // 5. Check for collisions (self or wall)
        if (detect_collisions()) {
            return score; // Game over, return final score
        }
        
        // 6. Check for food consumption
        if (snake[0][0] == food_y && snake[0][1] == food_x) {
            score++;
            snake_length++; // Snake grows
            
            // Clear the food position (though it should already be the new head)
            GLCD_DisplayChar(food_y, food_x, __FI, ' ');

            generate_food(); // Generate new food
            
            // Update score display
            GLCD_SetBackColor(White);
            GLCD_SetTextColor(Black);
            GLCD_DisplayString(10, 0, __FI, (unsigned char *)"SCORE: ");
            GLCD_DisplayChar(10, 7, __FI, (char)('0' + score)); // Display single-digit score
        }

        // 7. Pause/Delay
        delay();
    }
}