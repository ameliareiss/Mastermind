/*
Amelia Reiss
client.c

This client program connects to the server in order to play the game
mastermind. The client prints the rules and takes user guesses, 
sending it to the server for feedback. Once feedback is received, 
the client prints the current state of the board, and continues
asking for user input until the user enters "quit" or the game ends.

*/

#include "csapp.h"
#include "game.h"


int main(int argc, char *argv[]) {
    int clientfd;
    char *host, *port;
    size_t n;
    Game game;
    char buffer[MAXLINE]; // MAXLINE = 8192 -- defined in csapp.h

    // make sure appropriate amount of args are present
    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	    exit(0);
    }
    host = argv[1];
    port = argv[2];

    // wrapper function to call client functions
    clientfd = Open_clientfd(host, port);

    // print introduction
    printf("Welcome to Mastermind\n\n");
    print_rules();
    
    // ask for user input until they terminate the program
    int connected = 1;
    while (connected) {
        // make sure user input is correctly formatted
        int valid = 0;
        while (!valid) {
            // reset buffer and get message from user
            printf("\nEnter your guess with each digit separated by a space ex.\"1 2 3 4\"\n(enter \"quit\" to quit)\n");
            printf("Enter your guess: ");
            bzero(buffer, MAXLINE);
            Fgets(buffer, MAXLINE, stdin);
            valid = valid_guess(buffer);

            // if user wants to quit
            if (strncmp(buffer, "quit\n", 5) == 0) {
                connected = 0;
                break;
            }
        }
        if (!connected) {break;} 

        // send the data to the server, checking for errors
        n = write(clientfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("ERROR: Could not write to server.");
            return(-1);
        }

        // reset buffer and game state
        bzero(buffer,MAXLINE);
        bzero(&game, sizeof(Game));

        // read results from server
        n = read(clientfd, &game, sizeof(Game));
        if(n < 0) {
            perror("ERROR: Could not read from server.");
            return(-1);
        }

        // print results
        print_board(&game);
        Fputs(buffer, stdout);  

        // check win condition
        if (game_over(&game)) {
            connected = 0;
        }
    }
    
    printf("GAME OVER\n");
    Close(clientfd);
    return 0;
}
