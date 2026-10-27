#include "../Util/socketutil.h"
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

void ListenAndPrint(int SocketFD);
void startListeningMessagesOnNewThread(int fd);
int main(){
     // Create an IPv4 TCP socket
    int socketFD=createTCPIpv4Socket();

    struct sockaddr_in* address= createIPv4Address("127.0.0.1",2000);
    
    // Connect to the server
    int result = connect(socketFD,address,sizeof(*address));
    if(result==0){
        printf("connection was successful.\n");
    }
    startListeningMessagesOnNewThread(socketFD);

    char buffer[1024]; 

    char*line=NULL;
    size_t lineSize=0;
    printf("type and we will send..\n");
    
    
    char*name=NULL;
    size_t nameSize=0;
    printf("Please enter your name: ");
    ssize_t nameCount=getline(&name,&nameSize,stdin);
    name[nameSize-1]=0; 
    
    
  
    while(true){
       sprintf(buffer,"%s:$s",name,line);
       ssize_t charCount=getline(&line,&lineSize,stdin); 
       line[charCount-1]=0;
       if(charCount>0){
        if(strcmp(line,"exit\n")==0){
            break;
        }
        ssize_t amountWasSent=send(socketFD,line,charCount,0);
       
    } 
}

return 0;
}

void startListeningMessagesOnNewThread(int fd){
   
    pthread_t id;
    pthread_create(&id,NULL,ListenAndPrint,fd);
    
    
}
void ListenAndPrint(int SocketFD)
{   
    
    char buffer[1024];

    while (true)
    {
        ssize_t amountRecevied = recv(SocketFD, buffer, 1024, 0);
        if (amountRecevied > 0)
        {
            buffer[amountRecevied] = 0;
            printf("response was %s\n", buffer);

            
        }
        if (amountRecevied <= 0)
        {
            break;
        }
    }

    close(SocketFD);
}