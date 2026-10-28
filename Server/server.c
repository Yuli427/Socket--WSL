#include "server.h"
#include <pthread.h>
#include <unistd.h>


// Store information about all connected clients
struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;


//accept new client connection
void startAcceptingIncomingConnections(int serverSocketFD){

    while (true)

    {   // Accept a new client
        struct AcceptedSocket*clientSocket=acceptIncomingConnection(serverSocketFD);

        // Store the connected client
        acceptedSockets [acceptedSocketsCount++]=*clientSocket;
        
        receiveAndPrintIncomingDataOnSeperateThread(clientSocket);
    }

}

// Accept a new client connection and store its information
struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD){

    struct sockaddr_in clientAddress;
    int clientAddressSize=sizeof(clientAddress);

    // Accept a connection and get a new socket for this client
    int clientSocketFD=accept(serverSocketFD,&clientAddress,&clientAddressSize);
    
    // Allocate memory to store the client's connection information
    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address=clientAddress;
    acceptedSocket->acceptedSocketFD=clientSocketFD;
    acceptedSocket->acceptedSuccessfully=clientSocketFD>=0;
    
    
    // Store the error value if accept() failed
    if(!acceptedSocket->acceptedSuccessfully){
        acceptedSocket->error=clientSocketFD; 
    }
    return acceptedSocket;
}

// Start a new thread to receive messages from client
void receiveAndPrintIncomingDataOnSeperateThread(struct AcceptedSocket*clientSocket){
    
    pthread_t id;
    pthread_create(&id,NULL,receiveAndPrintIncomingData,clientSocket->acceptedSocketFD);
}



    void receiveAndPrintIncomingData(int socketFD)
    {
        char buffer[1024];
        char username[50];
        char joinMessage[100];
        char leaveMessage[100];

        ssize_t nameRecevied=recv(socketFD, username, 49, 0);
        if (nameRecevied > 0){

            username[nameRecevied] = '\0';
            sprintf(joinMessage,"%s is joined!\n", username);
            printf(joinMessage);
            //broadcast the join
            sendReceiveMessageToTheOtherClients(joinMessage,socketFD);
            
        }
        setSocketUsername(socketFD,username);
    

        while (true)
        {   
            // Receive a message from the client
            ssize_t amountRecevied = recv(socketFD, buffer, 1024, 0);

            if (amountRecevied > 0)
            {  
                buffer[amountRecevied] = 0;

                if(strcmp(buffer,"exit")==0){
                
                char*leavingUsername=getSocketUsername(socketFD);
                sprintf(leaveMessage,"%s is left.\n",leavingUsername);
                printf(leaveMessage);
                sendReceiveMessageToTheOtherClients(leaveMessage,socketFD);

                break;
                }
                
                // Print the received message on the server
                printf("%s", buffer);
                // Send the message to all other connected clients
                sendReceiveMessageToTheOtherClients(buffer,socketFD);
                
            }
            if (amountRecevied <= 0)
            {
                char*leavingUsername=getSocketUsername(socketFD);
                sprintf(leaveMessage,"%s is left.\n",leavingUsername);
                printf(leaveMessage);
                sendReceiveMessageToTheOtherClients(leaveMessage,socketFD);
                break;
            }
        }
    
    close(socketFD);
}

//broadcast the message
int sendReceiveMessageToTheOtherClients(char*buffer,int socketFD){
   // Do not send the message back to the original sender
  for(int i=0;i<acceptedSocketsCount;i++){
    if (acceptedSockets[i].acceptedSocketFD!=socketFD){
        send(acceptedSockets[i].acceptedSocketFD,buffer,strlen(buffer),0);
    }
  }

}

//helper
void setSocketUsername(int socketFD, char*name){

        for(int i=0;i<acceptedSocketsCount;i++){
        if( acceptedSockets[i].acceptedSocketFD==socketFD){
           strcpy(acceptedSockets[i].username,name);
        }
    }
    
}

char* getSocketUsername(int socketFD){

     for(int i=0;i<acceptedSocketsCount;i++){
        if( acceptedSockets[i].acceptedSocketFD==socketFD){
           return acceptedSockets[i].username;
        }
    }
}
