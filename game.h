/*
game.h

This header file contains function prototypes for the game logic.
*/

#ifndef GAME_H
#define GAME_H

#define MAX_CODE_LENGTH 6
#define NUM_GUESSES 12

// struct for game settings
typedef struct {
    int num_colors; // number of color options
    int code_length; // length of the code
    int repeats_allowed; // 1 if color repeats are allowed, 0 if not allowed
    int code[MAX_CODE_LENGTH]; // secret code
    int guesses[NUM_GUESSES][MAX_CODE_LENGTH]; // 2D array to hold previous guesses
    int feedback[NUM_GUESSES][MAX_CODE_LENGTH]; // 2D array to hold feedback for guesses
    int turn; // number of guesses made so far
} Game;

// function prototypes
void setup_game(Game *game, int num_colors, int code_length, int repeats_allowed);
void print_board(Game *game);
void generate_feedback(Game *game);
void update_guess(Game *game, char* input);
int valid_guess(char *input);
int game_over(Game *game);
void print_rules();


#endif // GAME_H