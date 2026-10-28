#include "../Util/socketutil.h"
#include "client.h"
#include <stdbool.h>



int main(){
    // Create an IPv4 TCP socket
    int socketFD=createTCPIpv4Socket();
    
    //create the server address using localhost and port 2000
    struct sockaddr_in* address= createIPv4Address("127.0.0.1",2000);
    
    // Connect the client socket to the server
    int result = connect(socketFD,address,sizeof(*address));
    if(result==0){
        printf("connection was successful.\n");
    }

    //start a seperate thread to listen for message from the server
    startListeningMessagesOnNewThread(socketFD);
    
    //create a buffer for store final message
    char buffer[1024];
    

    // store the user's message input
    char*line=NULL;
    size_t lineSize=0;
    printf("type and we will send..\n");
    
    // store the user's name
    char*name=NULL;
    size_t nameSize=0;
    printf("Please enter your name: ");
    ssize_t nameCount=getline(&name,&nameSize,stdin);
    name[nameCount-1]=0; 
    
    //send name
    send(socketFD,name,strlen(name),0);
  
    while(true){
       
        //read message from user
       ssize_t charCount=getline(&line,&lineSize,stdin); 

        //combine username and message
       sprintf(buffer,"%s:%s",name,line);
        //remove new line character
       line[charCount-1]=0;

       if(charCount>0){
        if(strcmp(line,"exit")==0){ 

            break;
        }
        ssize_t amountWasSent=send(socketFD,buffer,strlen(buffer),0);
       
    } 
}

   return 0;
}

