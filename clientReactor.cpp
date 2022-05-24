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

void* thread_func_recv(void* arg)
{
/* this function is the 'RUN' function for the thread that responsible on RECV */
    char buff[1024] = {0};
    // keep reciving meanwhile its not the end of the msges, the end msg will hold 0 bytes (as we defined at sender)
    connect_flag = 1; int bytes = 0;
    while (1)
    {
        if (!recv(sock, buff, sizeof(buff) + 1, 0))
            {
                close(sock);
                return 0;
            }
            for (int i = 0; i < 1024; i++)
            {
                printf("t%c", buff[i]);
            }
        // bytes = recv(sock, buff, 1024, 0);
        // if (bytes < 0) // this block checks if the client has been disconnected, so shall iterate at the outer loop 1 more (to accept command)
        // {
        //     connect_flag = 0;
        //     break;
        // }
        // printf("string is: %s\n", buff); // printing whats we recieved
        if (!strcmp(buff, "exit"))
        {
            connect_flag = 0;
            break;
        }
        bzero(buff,1024);     
    }
    printf("im here\n");
    return NULL;
}

void* thread_func_send(void* arg)
{
    /* this func shall send the user input to the server */
    char input[1024] = {0};
    char buff[1024] = {0};
    while(connect_flag != 0) 
    {
        if (!scanf("%[^\n]%*c", input))
        {
            while (getchar() != '\n')
                ;
        }
            if (send(sock, input, strlen(input) + 1, 0) == -1)
            {
                perror("send");
            }
            bzero(input, 1024);
            if (!recv(sock, input, sizeof(input) + 1, 0))
            {
                close(sock);
                return 0;
            }
            for (int i = 0; i < 1024; i++)
            {
                printf("%c", input[i]);
            }
            printf("\n");
    }
    return NULL;
}

int main(int argc, char **argv) 
{
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

    // connect to a server
    int clientSocket = connect(sock, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    if (clientSocket == -1) // fail => end of experiment
    {
        printf("listen failed");
        close(sock);
        return -1;
    }

    pthread_t pair_threads[2];

    connect_flag = 1;
    // pthread_create(&pair_threads[0], NULL, thread_func_recv, NULL);
    pthread_create(&pair_threads[1], NULL, thread_func_send, NULL);
    pthread_join(pair_threads[0], NULL);
    pthread_join(pair_threads[1], NULL);
    // pthread_kill(pair_threads[1], 0);

    close(sock); // close socket 
    printf("socket has been closed, the programme is done\n");
    return 0; 
}