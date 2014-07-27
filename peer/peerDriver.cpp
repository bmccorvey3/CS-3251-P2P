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

  	peerNode.printPrompt();
  	while(1){
  		//Receive Updates/Drops from Server

	  	peerNode.receiveFromServer();


	  	//Receive Data from the 1 previous peer

	  	peerNode.receiveFromPeers(primaryPort);


	  	//Receive Data from the 2 previous peer

	  	peerNode.receiveFromPeers(secondaryPort);


	  	//Send data to the primary and secondary recipients

	  	peerNode.sendToPeers();


	  	//Take in UI commands and send server commands and Queue TextMessages

	  	peerNode.operateUI();

	 }

}
