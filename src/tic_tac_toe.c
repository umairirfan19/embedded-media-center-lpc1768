#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "GLCD.h"
#include "KBD.h"
#include "Board_Buttons.h"
#include "tic_tac_toe.h"

#define NEUTRAL 0
#define PRESSED 1

// Game board grid array: 0=Empty, 1='X', 2='O'
// Rows 0-2, Columns 0-2
static int board[3][3];

// Cursor position (uses 0, 1, or 2)
static int cursor_row = 1;
static int cursor_col = 1;

// Current player: 1='X', 2='O'
static int current_player = 1;

// Game state: 0=Running, 1=X Wins, 2=O Wins, 3=Draw
static int game_state = 0;

// LCD mapping for the 3x3 grid (using approximate screen rows/cols for the 16x24 font)
#define BOARD_START_ROW 4
#define BOARD_START_COL 6
#define ROW_SPACING 2
#define COL_SPACING 4
#define JOYSTICK_DEBOUNCE_DELAY 3000000 // Delay loop size

int r, c, full;

// Helper: Delay function (copied from snake.c style)
void delay_ttt() {
    int i;
    for (i = 0; i < JOYSTICK_DEBOUNCE_DELAY; i++) {
        __nop();
        __nop();
    }
}

// Draw the 'X', 'O', or cursor marker at a given board position
// is_cursor: 1 if drawing the flashing cursor, 0 otherwise
void draw_cell(int r, int c, int value, int is_cursor) {
    char symbol;
    uint16_t color;
    
    int lcd_row = BOARD_START_ROW + r * ROW_SPACING;
    int lcd_col = BOARD_START_COL + c * COL_SPACING;
    
    if (value == 1) { // X
        symbol = 'X';
        color = Black;
    } else if (value == 2) { // O
        symbol = 'O';
        color = DarkGreen;
    } else { // Empty
        symbol = is_cursor ? '*' : ' '; // Use '*' as the cursor marker
        color = is_cursor ? DarkGrey : White;
    }
    
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(color);
    GLCD_DisplayChar(lcd_row, lcd_col, __FI, symbol);
    GLCD_SetTextColor(Black);
}

// Draw the board lines and static text
void tic_tac_toe_draw_board(void) {
    GLCD_Clear(White);
    GLCD_SetBackColor(Red);
    GLCD_SetTextColor(White);
    GLCD_DisplayString(0, 0, __FI, (unsigned char *)"T I C-T A C-T O E");
    GLCD_SetBackColor(White);
    GLCD_SetTextColor(Red);
    
    // --- Draw grid lines using text characters (quick and simple) ---
    GLCD_SetTextColor(Red);
    
    // Draw vertical lines
    GLCD_DisplayString(4, 8, __FI, (unsigned char *)"|");
    GLCD_DisplayString(5, 8, __FI, (unsigned char *)"|");
    GLCD_DisplayString(6, 8, __FI, (unsigned char *)"|");
    GLCD_DisplayString(7, 8, __FI, (unsigned char *)"|");
    GLCD_DisplayString(8, 8, __FI, (unsigned char *)"|");
    
    GLCD_DisplayString(4, 12, __FI, (unsigned char *)"|");
    GLCD_DisplayString(5, 12, __FI, (unsigned char *)"|");
    GLCD_DisplayString(6, 12, __FI, (unsigned char *)"|");
    GLCD_DisplayString(7, 12, __FI, (unsigned char *)"|");
    GLCD_DisplayString(8, 12, __FI, (unsigned char *)"|");
    
    // Draw horizontal lines
    GLCD_DisplayString(5, 4, __FI, (unsigned char *)"  ----------");
    GLCD_DisplayString(7, 4, __FI, (unsigned char *)"  ----------");
}

// Initialize the game state
void tic_tac_toe_init(void) {
    int r, c;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            board[r][c] = 0; // Empty
        }
    }
    cursor_row = 1;
    cursor_col = 1;
    current_player = 1; // X always starts
    game_state = 0; // Running
}

// Function to check for a winner (1: X wins, 2: O wins, 3: Draw, 0: Running)
int check_winner(void) {
    int r, c;
    
    // Check Rows and Columns
    for (r = 0; r < 3; r++) {
        // Check Row r
        if (board[r][0] != 0 && board[r][0] == board[r][1] && board[r][1] == board[r][2]) {
            return board[r][0];
        }
        // Check Column r
        if (board[0][r] != 0 && board[0][r] == board[1][r] && board[1][r] == board[2][r]) {
            return board[0][r];
        }
    }
    
    // Check Diagonals
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
    }
    
    // Check for Draw (if no winner and board is full)
    full = 1;
    for (r = 0; r < 3; r++) {
        for (c = 0; c < 3; c++) {
            if (board[r][c] == 0) {
                full = 0;
                break;
            }
        }
    }
    if (full) return 3; // Draw
    
    return 0; // Still running
}

// Main game loop function
int tic_tac_toe_game(void) {
    int input;
    int prev_input = NEUTRAL;
    
    tic_tac_toe_init();
    tic_tac_toe_draw_board();
    
    // Display instructions
    GLCD_DisplayString(2, 0, __FI, (unsigned char *)"R/L/U/P: Move | SEL");
    //GLCD_DisplayString(3, 0, __FI, (unsigned char *)"UP/DOWN (post-game): Exit/Restart");
    GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Player 1 starts!       ");

    // Initial draw of all cells to ensure board is clean and ready for cursor
    for(r=0; r<3; r++) {
        for(c=0; c<3; c++) {
            draw_cell(r, c, board[r][c], 0);
        }
    }
    
    // Main game loop
    while (1) {
        input = get_button();
        
        // --- 1. HANDLE INPUT AND CURSOR MOVEMENT (Only if game is running) ---
        if (game_state == 0) {
            
            // Redraw the cell at the old cursor position (removing the '*')
            draw_cell(cursor_row, cursor_col, board[cursor_row][cursor_col], 0);

            // Update cursor position based on joystick
            if (input == KBD_UP) {
                cursor_row = (cursor_row > 0) ? cursor_row - 1 : 0;
            } else if (input == KBD_DOWN) {
                cursor_row = (cursor_row < 2) ? cursor_row + 1 : 2;
            } else if (input == KBD_LEFT) {
                cursor_col = (cursor_col > 0) ? cursor_col - 1 : 0;
            } else if (input == KBD_RIGHT) {
                cursor_col = (cursor_col < 2) ? cursor_col + 1 : 2;
            }

            // Draw current cursor position
            if (board[cursor_row][cursor_col] == 0) {
                 draw_cell(cursor_row, cursor_col, 0, 1); // Draw '*' cursor
            } else {
                // If cell is marked, just redraw the mark (no cursor)
                draw_cell(cursor_row, cursor_col, board[cursor_row][cursor_col], 0);
            }
            
            // Handle placement on SELECT/PRESSED
            if (input == PRESSED && prev_input != PRESSED) {
                if (board[cursor_row][cursor_col] == 0) {
                    // Place the mark
                    board[cursor_row][cursor_col] = current_player;
                    draw_cell(cursor_row, cursor_col, current_player, 0); // Draw permanent X/O
                    
                    // Check for win/draw
                    game_state = check_winner();
                    
                    if (game_state == 0) {
                        // Switch player
                        current_player = (current_player == 1) ? 2 : 1;
                        GLCD_DisplayString(2, 0, __FI, (unsigned char *)((current_player == 1) ? "Player 1's Turn       " : "Player 2's Turn       "));
                    }
                }
            }
        }
        
        // --- 2. HANDLE GAME END STATE ---
        if (game_state != 0) {
						GLCD_Clear(White);
            GLCD_SetTextColor(Black);
            GLCD_SetBackColor(White);
            
            if (game_state == 1) {
                GLCD_SetTextColor(Black);
                GLCD_DisplayString(2, 0, __FI, (unsigned char *)"Player 1 WINS!        ");
            } else if (game_state == 2) {
                GLCD_SetTextColor(DarkGreen);
                GLCD_DisplayString(2, 0, __FI, (unsigned char *)"Player 2 WINS!        ");
            } else if (game_state == 3) {
                GLCD_SetTextColor(Red);
                GLCD_DisplayString(2, 0, __FI, (unsigned char *)"GAME DRAWN!           ");
            }
            
            // Wait for input to exit/restart
            GLCD_SetTextColor(Black);
            GLCD_DisplayString(4, 0, __FI, (unsigned char *)"UP: START AGAIN");
            GLCD_DisplayString(5, 0, __FI, (unsigned char *)"DOWN: EXIT");

            if (input == KBD_UP && prev_input != KBD_UP) {
                // Restart
                tic_tac_toe_init();
                tic_tac_toe_draw_board();
                GLCD_DisplayString(3, 0, __FI, (unsigned char *)"Player 1 (Black) starts!       ");
            } else if (input == KBD_DOWN && prev_input != KBD_DOWN) {
                // Exit
                return 0; // Return 0 (success) to main loop to trigger exit
            }
        }
        
        prev_input = input;
        delay_ttt();
    }
}