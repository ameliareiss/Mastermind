# Mastermind

A concurrent, client-server based implementation of the classic board game Mastermind. The server generates a 4-digit code, and the player must crack the code within 10 guesses. The server sends feedback to the client, indicating the number of correct digits in the code, as well as the number of correctly positioned digits.

## Files

*   `server.c`: Source code for the server.
*   `client.c`: Source code for the client.
*   `game.c`, `game.h`: Source code related to game logic.
*   `csapp.c`, `csapp.h`: Helper functions from the CS:APP textbook.
*   `makefile`: Instructions for compiling the project.

## Building the Project

To build the project:

Open your terminal and navigate to the directory where you saved the project files. Then, run the `make` command.

For example:
```bash
cd /path/to/your/project/files
make
```

This will create two executable files: `client` and `server`.

## How to Run

You will need two terminal windows to run the application.

**1. Start the server:**

In the first terminal, run the following command, replacing `<port>` with a port number of your choice (e.g., 8080):
```bash
./server <port>
```

**2. Start the client:**

In the second terminal, run the following command. Replace `<host>` with the server's hostname (e.g., `localhost` if running on the same machine) and `<port>` with the same port number you used for the server:
```bash
./client <host> <port>
```
Once connected, follow the on-screen instructions to play the game.

## Cleaning Up

To remove the compiled executables and any temporary files, you can run:

```bash
make clean
``` 
