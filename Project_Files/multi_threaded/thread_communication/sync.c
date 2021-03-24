
/*-----------------------*
 *                       *
 *  Author: Andy Morato  *
 *  Date: 3/23/21        *
 *                       *
 *-----------------------*/

/* ---------------------------- Task ----------------------------- */

/*

- Create an inter-thread communication variable, like a condition,
  semaphore, etc., not a primitive. You can create this at the global
  level for simplicity

- In the initial thread:
     - Create a TCP server socket and listen for a connection
     - After a connection is accepted, send a few strings over the
       connection with a short delay between them
     - After sending each string, signal the inter-thread
       communication variable
     - After a few strings, close the connection
     - Wait for the second thread to exit then cleanup its resources
     - Return 0 from main()

- In a second thread:
     - Create a TCP client socket and connect to the server socket
     - After connecting, wait on the inter-thread communication
       variable
     - After the signal is successfully received, read from the
       client socket and print the string to the terminal
     - Repeat the wait and read steps until the connection is closed
     - Return from the thread function

- Use the loopback address for the connection

- Feel free to use Linux and any library you prefer for sockets or
  threads – pthread, boost, etc.  Just let me know the version so I
  can compile and link correctly

- If using Linux, provide a command line that will compile and link
  the program.

- (You could technically use the socket’s inherent blocking behavior
  to achieve the same results, but I would like to see the use of
  inter-thread communication. You could set the sockets to
  non-blocking to remove that temptation)
  
*/

/* --------------------- Imports and Globals --------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define LENGTH 5
#define NUM_OF_MSGS 4
#define PORT 59991
  
sem_t * semaphore;

/* ------------------- Client thread functions ------------------- */
  
void * client_thread()
{
    int sock;
    struct sockaddr_in client_address;
  
    // Socket creation.
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("[Client Thread] Socket creation failed\n");
        exit(0);
    } else {
        printf("[Client Thread] Socket creation successful\n");
    }
    bzero(&client_address, sizeof(client_address));
  
    // Set up the client address.
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    client_address.sin_port = htons(PORT);
  
    // Connect the client to server socket.
    if (connect(
            sock, 
            (struct sockaddr *) &client_address, 
            sizeof(client_address)) != 0) {
        printf("[Client Thread] Connection failed\n");
        exit(0);
    } else {
        printf("[Client Thread] Connection successful\n");
    }
  
    // Turn off blocking.
    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

    // Read messages from the server until the connection closes.
    int rc = 1;
    while (rc > 0) {
        char buffer[LENGTH];
        bzero(buffer, sizeof(buffer));
        sem_wait(semaphore);
        rc = read(sock, buffer, sizeof(buffer));
        if (rc > 0) {
            printf("[Client Thread] Message: %s\n", buffer);
        }
    }
  
    // Close the socket.
    printf("[Client Thread] Closing connection\n");
    close(sock);

    // Exit the thread.
    printf("[Client Thread] Exiting the thread\n");
    return NULL;
}

/* ------------------- Server thread functions ------------------- */

void server_connection()
{
    // Socket creation;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("[Server Thread] Socket creation failed\n");
        exit(0);
    } else {
        printf("[Server Thread] Socket creation successful\n");
    }

    // Set up the server address.
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
  
    // Bind socket.
    if ((bind(sock, 
              (struct sockaddr *) &server_address, 
              sizeof(server_address))) != 0) {
        printf("[Server Thread] Binding failed\n");
        exit(0);
    } else {
        printf("[Server Thread] Binding successful\n");
    }
  
    // Listen and accept connection.
    int fd;
    if ((listen(sock, 5)) != 0) {
        printf("[Server Thread] Listen failed\n");
        exit(0);
    } else {
        printf("[Server Thread] Listening for a connection\n");
    }
    if ((fd = accept(sock, NULL, NULL)) < 0) {
        printf("[Server Thread] Connection acceptance failed\n");
        exit(0);
    } else {
        printf("[Server Thread] Connection accepted\n");
    }
  
    // Turn off blocking.
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

    // Write 4 strings to the client thread.
    int i;
    char * strings[] = {"bowie", "queen", "elton", "elvis"};
    for (i = 0; i < NUM_OF_MSGS; i++) {
        sleep(2);
        printf("[Server Thread] Writing message to client\n");
        write(fd, strings[i], LENGTH);
        sleep(2);
        sem_post(semaphore);
    }
  
    // Close the socket.
    printf("[Server Thread] Closing connection\n");
    close(sock);
}
  
int main() 
{

    /* 
       Create a semaphore with a value of 5.
       Note: sem_unlink() is used to avoid carrying over the
             semaphore's value from previous executions.
             sem_init() is unavailable on OSX and sem_open() creates 
             semaphores whose value persists across different
             executions of the same program, regardless of its new
             assigned value, since sem_open()'s semaphores are
             intended to be shared among processes.
    */
    const char * name = "/semaphore";
    sem_unlink(name);
    semaphore = sem_open(name, O_CREAT, 0644, 0);

    // Launch client thread.
    printf("[Server Thread] Launching client thread\n");
    pthread_t t1;
    pthread_create(&t1, NULL, client_thread, NULL);

    // Begin server TCP connection.
    server_connection();
    sem_post(semaphore);

    // Wait until the client thread has exited.
    printf("[Server Thread] Waiting for client thread to exit\n");
    pthread_join(t1, NULL);

    // Close the semaphore.
    sem_close(semaphore);

    // End the program.
    printf("[Server Thread] Ending the program\n");
    return 0;
} 