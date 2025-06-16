/* 
Amelia Reiss
CSCI 3240
Final Project - Mastermind
server.c

This server program hosts and allows multiple client connections to play
the game of mastermind. It keeps track of when clients connect and 
disconnect, and it keeps track of the secret code used for each game.
The server receives guesses from each client, providing the respective
feedback for the guess, sending the current game state to the client.

*/

#include "csapp.h"
#include "game.h"
#include <pthread.h>

// used to pass args to threads
typedef struct {
    char *hostName;
    char *portNumber;
    int connfd;
} ThreadArgs;

// function prototypes
void *thread(void *vargp);


int main(int argc, char *argv[]) {
    // variables to set up server and concurrency
    int listenfd;
    int *connfdp;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; 
    pthread_t tid;
    char client_hostname[MAXLINE], client_port[MAXLINE];

    // make sure appropriate args were passed to set up server
    if (argc != 2) {
    	fprintf(stderr, "usage: %s <port>\n", argv[0]);
    	exit(0);
    }

    // wrapper function to call server functions
    listenfd = Open_listenfd(argv[1]);

    // ensure server runs indefinitely
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        
        // wait for the connection from the client
        connfdp = Malloc(sizeof(int));
    	*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname,
                                   MAXLINE,client_port, MAXLINE, 0);
        
        // create struct to pass args to thread
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->connfd = *connfdp;
        args->hostName = client_hostname;
        args->portNumber = client_port; 

        // create thread for client connection
        printf("Connected to (%s, %s)\n", client_hostname, client_port);
        pthread_create(&tid, NULL, thread, args);

        Free(connfdp);
    
    }
    
    return 0;
}

/* This function initializes the game for the client and allows the
server to receive guesses. It also sends the updated game state to the client. */
void server_function(int connfd, Game *game, int port) {
    // variables for client-server communication
    char buffer[MAXLINE]; // MAXLINE = 8192 defined in csapp.h
    size_t n;
    
    // initialize game state
    setup_game(game, 6, 4, 0);
    printf("The code for %d is:\n", port);

    // print secret code generated
    for (int i = 0; i < game->code_length; i++) {
        printf("%d ", game->code[i]);
    }
    printf("\n");

    while (1) {
        // reset the buffer
        bzero(buffer,MAXLINE);

        // receive guess from client
        n = read(connfd, buffer, MAXLINE);
        if (n < 0){
            perror("ERROR: Could not read message.");
            return;
        }

        // check if client disconnected
        if (n == 0) {
            printf("Client disconnected\n");
            break;
        }

        // check guess and update board
        update_guess(game, buffer);
        generate_feedback(game);
        game->turn++;
        
        // return updated game to client
        n = write(connfd, game, sizeof(Game));
        if (n < 0){
            perror("ERROR: Could not write to client.");
            return;
        }
    }
}

void *thread(void *vargp) {
    // get args from arg struct
    ThreadArgs *args = (ThreadArgs *)vargp; // convert void arg into struct arg
    int connfd = args->connfd;
    char *name = args->hostName;
    int port = atoi(args->portNumber);
    Game game; // create new game when thread is created

    // call server function
    pthread_detach(pthread_self());
    server_function(args->connfd, &game, port);
    
    // stop when client disconnects
    printf("(%s, %d) disconnected\n", name, port);
    Free(args);
    Close(connfd);
    return NULL;
}