#include "../Util/socketutil.h"
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

struct AcceptedSocket{
    int acceptedSocketFD;  // Client socket
    struct sockaddr_in address; // Client address
    int error;// Error value
    bool acceptedSuccessfully;// Whether accept() was successful
};

struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD);

void receiveAndPrintIncomingData(int socketFD);

void acceptNewConnection(int serverSocketFD);

void startAcceptingIncomingConnections(int serverSocketFD);

void receiveAndPrintIncomingDataOnSeperateThread(struct AcceptedSocket*clientSocket);

int sendReceiveMessageToTheOtherClients(char*buffer,int socketFD);





int main()
{
    int serverSocketFD= createTCPIpv4Socket();
    struct sockaddr_in *serverAddress=createIPv4Address("127.0.0.1",2000);

    int result=bind(serverSocketFD,serverAddress,sizeof(*serverAddress));

    if(result==0){
        printf("bound successfuly!\n");
    }

    int listen_result=listen(serverSocketFD,10);
    startAcceptingIncomingConnections(serverSocketFD);

    

    
    shutdown(serverSocketFD,SHUT_RDWR);
 
    return 0;
}

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;

void startAcceptingIncomingConnections(int serverSocketFD){

    while (true)
    {
        struct AcceptedSocket*clientSocket=acceptIncomingConnection(serverSocketFD);
        acceptedSockets [acceptedSocketsCount++]=*clientSocket;
        receiveAndPrintIncomingDataOnSeperateThread(clientSocket);
    }

}


void receiveAndPrintIncomingDataOnSeperateThread(struct AcceptedSocket*clientSocket){
    
    pthread_t id;
    pthread_create(&id,NULL,receiveAndPrintIncomingData,clientSocket->acceptedSocketFD);
}


void receiveAndPrintIncomingData(int socketFD)
{
    char buffer[1024];
    while (true)
    {
        ssize_t amountRecevied = recv(socketFD, buffer, 1024, 0);
        if (amountRecevied > 0)
        {
            buffer[amountRecevied] = 0;
            printf("response was %s\n", buffer);

            sendReceiveMessageToTheOtherClients(buffer,socketFD);
        }
        if (amountRecevied <= 0)
        {
            break;
        }
    }
    
    close(socketFD);
}
int sendReceiveMessageToTheOtherClients(char*buffer,int socketFD){

  for(int i=0;i<acceptedSocketsCount;i++){
    if (acceptedSockets[i].acceptedSocketFD!=socketFD){
        send(acceptedSockets[i].acceptedSocketFD,buffer,strlen(buffer),0);
    }
  }

}
struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD){

    struct sockaddr_in clientAddress;
    int clientAddressSize=sizeof(clientAddress);
    int clientSocketFD=accept(serverSocketFD,&clientAddress,&clientAddressSize);
    
    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address=clientAddress;
    acceptedSocket->acceptedSocketFD=clientSocketFD;
    acceptedSocket->acceptedSuccessfully=clientSocketFD>=0;

    if(!acceptedSocket->acceptedSuccessfully){
        acceptedSocket->error=clientSocketFD; 
    }
    return acceptedSocket;
}