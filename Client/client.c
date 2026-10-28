#include "client.h"
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdio.h>

// Start a new thread to listen for incoming messages from the server
void startListeningMessagesOnNewThread(int fd){
   
    pthread_t id;

    // Create a new thread that runs ListenAndPrint()
    pthread_create(&id,NULL,ListenAndPrint,fd);
    
    
}

// Receive and print messages from the server
void ListenAndPrint(int SocketFD)
{   
    
    char buffer[1024];

    while (true)
    {   
        //Receive data from the server
        ssize_t amountReceived = recv(SocketFD, buffer, 1024, 0);

        if (amountReceived > 0)
        {   

            //add null terminator - string
            buffer[amountReceived] = 0;
            printf("%s",buffer);

            
        }

        //stop listening if the connection is closed or error occurs
        if (amountReceived <= 0)
        {   
            break;
        }
    }
    
    close(SocketFD);
}