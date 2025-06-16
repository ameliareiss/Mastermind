/*
game.c

This file contains the implementation for the game logic.
*/

#include "game.h"
#include "csapp.h"

/* This function receives a game struct, the number of color/digit options for the
code, the length of the code, and if repeat digits are allowed in the code. It
initializes the game struct with these values, zeros out the arrays used for storing 
guesses and feedback, and generates the secret code.
*/
void setup_game(Game *game, int num_colors, int code_length, int repeats_allowed) {
    srand(time(NULL));
    game->num_colors = num_colors;
    game->code_length = code_length;
    game->repeats_allowed = repeats_allowed;
    game->turn = 0;
    
    // zero out arrays
    bzero(game->code, sizeof(game->code));
    bzero(game->guesses, sizeof(game->guesses));
    bzero(game->feedback, sizeof(game->feedback));

    // generate secret code
    for (int i = 0; i < game->code_length; i++) {
        game->code[i] = rand() % game->num_colors + 1;
    }
}

/* This function receive a game struct and prints the current state 
of the board, displaying previous guesses and their respective 
feedback, as well as the number of guesses remaining. */
void print_board(Game *game) {
    // print header
    printf("\nGUESS     FEEDBACK\n");
    
    for (int turn = 0; turn < NUM_GUESSES; turn++) {
        // print guess first
        for (int pos = 0; pos < game->code_length; pos++) {
            printf("%d ", game->guesses[turn][pos]);
        }
        
        // print feedback after
        printf("| ");
        for (int pos = 0; pos < game->code_length; pos++) {
            printf("%d ", game->feedback[turn][pos]);
        }
        printf("\n");
    }
}

/* This function receives the game struct and generates feedback
based on the secret code. It then updates the feedback for
the respective turn. */    
void generate_feedback(Game *game) {
    // generate feedback for guess
    int num2s = 0;
    int num1s = 0;
    int *guess = game->guesses[game->turn];
    
    // arrays used for tracking repeats
    int code_used[game->code_length];
    int guess_used[game->code_length];

    // initialize repeat arrays
    for (int i = 0; i < game->code_length; i++) {
        code_used[i] = 0;
        guess_used[i] = 0;
    }

    // check for correct color and correct position
    for (int i = 0; i < game->code_length; i++) {
        if (guess[i] == game->code[i]) {
            num2s++;
            code_used[i] = 1;
            guess_used[i] = 1;
        }
    }

    // check for correct color but incorrect position
    for (int i = 0; i < game->code_length; i++) {
        // skip guess if already matched exactly
        if (guess_used[i]) {continue;}
        
        // for each guess, check if code matches and hasnt been marked before
        for (int j = 0; j < game->code_length; j++) {
            if (!code_used[j] && game->code[j] == guess[i]) {
                num1s++;
                code_used[j] = 1;
                guess_used[i] = 1;
                break;
            }
        }
    }
    
    // format feedback and update game state
    int i = 0;
    while(num2s > 0 || num1s > 0) {
        if (num2s > 0) {
            game->feedback[game->turn][i] = 2;
            num2s--;
        }
        else if (num1s > 0) {
            game->feedback[game->turn][i] = 1;
            num1s--;
        }
        i++;
    }
}

/* This function receives a game struct and a string that is the user's
guess. It converts the string's componenets into digits and updates
the game state. */
void update_guess(Game *game, char* input) {
    char *token = strtok(input, " ");
    for (int i = 0; i < game->code_length; i++) {
        game->guesses[game->turn][i] = atoi(token);
        token = strtok(NULL, " ");
    }
}

/* This function receives a string that is the user's guess,
and it checks to make sure it is in the correct format.
ex. 1 2 3 4 (four digits separated by space)
It returns 1 if the guess is valid and 0 if not. */
int valid_guess(char *input) {
    if (strlen(input) != 8) {
        return 0;
    }
    for (int i = 0; i < 7; i++) {
        // even index is a digit
        if (i % 2 == 0 && !isdigit(input[i])) {
            return 0;
        } // odd index is whitespace
        else if (i % 2 != 0 && input[i] != ' '){
            return 0;} 
    }
    return 1;
}

/* This function checks whether the game is over, by
determinining if the last turn's guess was made, or if
the last feedback produced all 2's. */
int game_over(Game *game) {
    // check if this was the last guess
    if (game->turn == NUM_GUESSES) {
        return 1;
    }
    // check if feedback is all 2's
    for (int i = 0; i < game->code_length; i++) {
        if (game->feedback[game->turn-1][i] != 2) {
            return 0;
        }
    }
    return 1;
}

/* This function prints an overview of the rules of the game. */
void print_rules() {
    printf("The goal of the game is to decipher a 4-digit code, comprised of digits 1-6.\n");
    printf("The server will provide feedback for the guess in no particular order.\n");
    printf("Feedback:\n\t2: correct number in the correct position\n\t1: correct number");
    printf("but incorrect position\n\t0: number is not in the code.\n");
    printf("You have 12 guesses to break the code. Repeat digits are allowed.\n");
    
}