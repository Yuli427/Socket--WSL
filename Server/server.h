#include "../Util/socketutil.h"
#include<stdbool.h>

struct AcceptedSocket{
    int acceptedSocketFD;  // Client socket
    struct sockaddr_in address; // Client address
    int error;// Error value
    bool acceptedSuccessfully;// Whether accept() was successful
    char username[50];
};

struct AcceptedSocket* acceptIncomingConnection(int serverSocketFD);

void receiveAndPrintIncomingData(int socketFD);

void acceptNewConnection(int serverSocketFD);

void startAcceptingIncomingConnections(int serverSocketFD);

void receiveAndPrintIncomingDataOnSeperateThread(struct AcceptedSocket*clientSocket);

int sendReceiveMessageToTheOtherClients(char*buffer,int socketFD);

void setSocketUsername(int socketFD, char* name);

char* getSocketUsername(int socketFD);