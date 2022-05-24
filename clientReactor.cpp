#include <netinet/in.h>
#include <netinet/tcp.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <arpa/inet.h>
#define SERVER_PORT 9034
#define SERVER_IP_ADDRESS "127.0.0.1"

static int connect_flag = 0;
int sock = -1;

/*
        ----- ATTENTION!!!---- 
        given word 'magic word' EXIT will shut down the CLIENT
*/

void* thread_func_recv(void* vargp)
{
/* this function is the 'RUN' function for the thread that responsible on RECV */
    char buff[1024] = {0};
    // keep reciving meanwhile its not the end of the msges, the end msg will hold 0 bytes (as we defined at sender)
    connect_flag = 1; int bytes = 0;
    while ((bytes = recv(sock, buff, 1024, 0)) != -1 )
    {
        if (bytes == 0) // this block checks if the client has been disconnected, so shall iterate at the outer loop 1 more (to accept command)
        {
            connect_flag = 0;
            break;
        }
        printf("%s\n", buff); // printing whats we recieved
        if (!strcmp(buff, "exit"))
        {
            connect_flag = 0;
            break;
        }
        bzero(buff,1024);     
    }
    return NULL;
}

void* thread_func_send(void* vargp)
{
    /* this func shall send the user input to the server */
    char input[1024] = {0};
    while(connect_flag != 0) 
    {
        if (!scanf("%[^\n]%*c", input))
        {
            while (getchar() != '\n');
        }
        if (strcmp(input, "exit") == 0) // sending EXIT to server.. this way thread_recv will gain "EXIT" and will get to end as well
        {
            send(sock, "exit", 4, 0);
            connect_flag = 0;
            break;
        }
        if(send(sock, input, strlen(input), 0) == -1) // send w.e we got as input from user
        {
            return NULL;
        }
    }
    return NULL;
}

int main(int argc, char **argv) 
{
    /* this little programme represents a client that connects to local server and works on 2 threads
        1- recv -> which waits for server msgs and prints them on screen
        2- send -> waits for input from user and sends it to the server

        ----- ATTENTION!!!---- 
        given word 'magic word' EXIT will shut down the CLIENT
    */
    // create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) {
            perror("socket");
    return -1;
    } 
    signal(SIGPIPE, SIG_IGN); // on linux to prevent crash on closing socket
    
    // open socket
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    // init client
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
    serverAddress.sin_port = htons(SERVER_PORT);  //network order

    printf("client sock is on ^^\n");

    // connect to a server
    int clientSocket = connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (clientSocket == -1) // fail => end of experiment
    {
        printf("listen failed");
        close(sock);
        return -1;
    }

    pthread_t pair_threads[2]; // [0] to recv, [1] to send

    connect_flag = 1;
    // exec rcv, send operations threads
    pthread_create(&pair_threads[0], NULL, thread_func_recv, NULL);
    pthread_create(&pair_threads[1], NULL, thread_func_send, NULL);
    
    // join both threads
    pthread_join(pair_threads[0], NULL);
    pthread_kill(pair_threads[1], 0);

    close(sock); // close socket 
    printf("socket has been closed, the programme is done\n");
    return 0; 
}