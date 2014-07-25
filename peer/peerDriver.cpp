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

static p_thread_mutex_t sendRecieveMutex;

int main(int argc, char **argv){
	if (argc < 2)
  {
    fprintf(stderr,
            "Usage: peer SERVER ""\n");
    exit(1);
  }
  	serverChar = argv[1];
  	server(serverChar);
  	Peer peerNode = new Peer(server);

  	p_thread_mutex_init(&sendReceiveMutex, NULL);
  	while(1){
  		//Receive Updates/Drops from Server
	  	p_thread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromServer();
	  	p_thread_mutex_unlock(&sendReceiveMutex);

	  	//Receive Data from the 1 previous peer
	  	p_thread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromPeers(primaryPort);
	  	p_thread_mutex_unlock(&sendReceiveMutex);

	  	//Receive Data from the 2 previous peer
	  	p_thread_mutex_lock(&sendReceiveMutex);
	  	peerNode.receiveFromPeers(secondaryPort);
	  	p_thread_mutex_unlock(&sendReceiveMutex);

	  	//Send data to the primary and secondary recipients
	  	p_thread_mutex_lock(&sendReceiveMutex);
	  	peerNode.sendToPeers();
	  	p_thread_mutex_unlock(&sendReceiveMutex);

	  	//Take in UI commands and send server commands and Queue TextMessages
	  	p_thread_mutex_lock(&sendReceiveMutex);
	  	peerNode.sendToServer();
	  	p_thread_mutex_unlock(&sendReceiveMutex);

	 }

}