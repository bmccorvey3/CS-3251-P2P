#include <stdio.h>
#include <stdlib.h>             
#include <unistd.h>          
#include <assert.h>       
#include <signal.h>       
#include <alloca.h>  
#include <pthread.h>

#define primaryPort 22222
#define secondaryPort 33333

char *serverChar;

static pthread_mutex_t sendReceiveMutex;

int main(int argc, char **argv){
	if (argc < 2)
  {
    fprintf(stderr,
            "Usage: peer SERVER ""\n");
    exit(1);
  }
  	serverChar = argv[1];
  	std::string server(serverChar);
  	Peer peerNode = new Peer(server);

  	pthread_mutex_init(&sendReceiveMutex, NULL);
  	peerNode.printPrompt();
  	while(1){
  		//Receive Updates/Drops from Server
	  	pthread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromServer();
	  	pthread_mutex_unlock(&sendReceiveMutex);

	  	//Receive Data from the 1 previous peer
	  	pthread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromPeers(primaryPort);
	  	pthread_mutex_unlock(&sendReceiveMutex);

	  	//Receive Data from the 2 previous peer
	  	pthread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromPeers(secondaryPort);
	  	pthread_mutex_unlock(&sendReceiveMutex);

	  	//Send data to the primary and secondary recipients
	  	pthread_mutex_lock(&sendReceiveMutex);
	  	peerNode.sendToPeers();
	  	pthread_mutex_unlock(&sendReceiveMutex);

	  	//Take in UI commands and send server commands and Queue TextMessages
	  	pthread_mutex_lock(&sendReceiveMutex);
	  	peerNode.sendToServer();
	  	pthread_mutex_unlock(&sendReceiveMutex);

	 }

}
