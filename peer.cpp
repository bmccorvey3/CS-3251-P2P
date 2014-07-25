#include "peer.h"
#include "sha256.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <iostream.h>

#define BUFSIZE 1024

Peer::Peer()
{
	queue<String> m_messageHashes;
	queue<String> m_messageList;

	char *string username;
	char *string portNumber;
	char *string ServerName;

	IPAddr primaryRecipient;
	IPAddr secondaryRecipient;

	static p_thread_mutex_t sendReceiveMutex;

	int sock;
	int recv_sock;
	int cli_sock;
	socklen_t recipient_len;
	socklen_t svr_len;
	struct sockaddr_in peer_addr; //Recieving from Peers
    struct sockaddr_in recipient_addr; //The Peers sending to this
    struct sockaddr_in cli_addr; //This Client sending Response to Server
    struct sockaddr_in svr_addr; //Server Sending Infor the this client
    struct sockaddr_in addr; //This client sending data to server
    struct sockaddr_in addr1; //Recipient 1
    struct sockaddr_in addr2; //Recipient 2
    char buffer[BUFSIZE];
}

Peer::~Peer()
{
	delete queue<String> m_messageHashes;
	delete queue<String> m_messageList;

	delete char *string username = NULL;
	delete char *string portNumber;
	delete char *string ServerName;

	delete IPAddr primaryRecipient;
	delete IPAddr secondaryRecipient;
}

int main(int argc, char **argv){
	if (argc < 2)
  {
    fprintf(stderr,
            "Usage: tcp_client SERVER ""\n");
    exit(1);
  }
  g_szServer = argv[1];

  g_usPort = 11111;
  setupSocket();
  p_thread_mutex_init(&sendReceiveMutex, NULL);

	while(1){
  	//Receiving from second previous Peer
		pthread_mutex_lock(&sendReceiveMutex);
		setupReceiveFromPeersSocket();
		recipient_len = sizeof(recipient_addr);
		int recipient_sock = accept(peer_sock, (struct sockaddr_in*)&recipient_addr, &recipient_len);
		memset(buffer, '\0', sizeof(buffer));
		while(recvfrom(recipient_sock, buffer, BUFSIZE, 0, (struct sockaddr_in*)&recipient_addr, &recipient_len)>0){
			String hashed = hashMessage(&buffer);
			if(!m_messageHashes.contains(hashed)){
				printf("%s", buffer);
				fflush(stdin);
				fflush(stdout);

				m_messageHashes.push(hashed); //Put in hashed table of received messages
				m_messageList.push(buffer); //Put into queue to push to rest of clients
			}
			else{

			}
	  }
	  close(recipient_sock);
	  close(peer_sock);
	  pthread_mutex_unlock(&sendReceiveMutex);

	//Receive From Server
		pthread_mutex_lock(&sendReceiveMutex);
		setupReceiveFromServerSocket();
		svr_len = sizeof(svr_addr);
		svr_sock = accept(cli_sock, (struct sockaddr_in*)&rsvr_addr, &svr_len);
		memset(buffer, '\0', sizeof(buffer));
		int changed = 0;
		while(recvfrom(svr_sock, buffer, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){
			updateRecipients(buffer, svr_sock);
			changed =1;
		}
		close(svr_sock);
		close(cli_sock);
		pthread_mutex_unlock(&sendReceiveMutex);

	//Sending to other peers
		pthread_mutex_lock(&sendReceiveMutex);
		setupSendToPeersSocket();
		while(m_messageList.size() > 0){ //Sends everything stored in the buffer to the two peers.
			//Buffer gains messages from receivePeers and from stdin with "text" CODE
			buffer = m_messageList.pop();
			while(sendto(sock1, buffer, strlen(buffer), 0, (struct sockaddr_in*)&addr1, sizeof(addr1))){};
			while(sendto(sock2, buffer, strlen(buffer), 0, (struct sockaddr_in*)&addr2, sizeof(addr2))){};
		}
		close(sock1);
		close(sock2);
		pthread_mutex_unlock(&sendReceiveMutex);

	//Writing to the STDIN, meaning messages to Server, or writing to the message buffer
		pthread_mutex_lock(&sendReceiveMutex);
		char *command;
		char *code;
		char *message;
		char *search = " ";
		while(1){
			if(getline(&command, NULL, stdin) == -1){
				command = "0000";
			}
			else{
				code = strtok(command,search); //Cuts off at first space Code: Message
				message = strtok(NULL, search)
			}
			if(strncmp(code, "text")){
				Textmsg message = new Textmsg(message);
				hashed = hashMessage(message);
				m_messageHashes.push(hashed);
				m_messageList.push(message);
			}
			if(strncmp(code, "lvps")){
				leaveChatroom();
			}
			if(strncmp(code, "enps")){
				enterChatroom(message);
			}
			if(strncmp(code, "crps")){
				createChatroom(message);
			}
			if(strncmp(code, "lsps")){
				listChatrooms();
			}
			if(strncmp(code, "dsps")){
				destroyChatroom(message);
			}
		}
		pthread_mutex_unlock(&sendReceiveMutex);

	}
}

void leaveChatroom(){
	setupSendToServerSocket();
	LeaveChatroomMsg leaving = new LeaveChatroomMsg();
	close(sock);
}

void enterChatroom(char *chatroom){
	setupSendToServerSocket();
	EnterChatroomMsg entering = new EnterChatroomMsg(chatroom);


	if()//Response if Username is needed
		createUsername(username);
	close(sock);
}

void createUsername(char *name){
	username = name;
	ChooseUsernameMsg name = new ChooseUsernameMsg(username);
}

void createChatroom(char *chatroom){
	setupSendToServerSocket();
	CreateChatroomMsg creating = new CreateChatroomMsg(chatroom);
	close(sock);
}

void listChatrooms(){
	setupSendToServerSocket();
	ListChatroomMsg listing = new ListChatroomMsg();
	close(sock);
}

void destroyChatroom(char *chatroom){
	setupSendToServerSocket();
	destroyChatroomMsg destroying = new destroyChatroomMsg(chatroom);



	if()//Response if chatroom destroyed or not
	close(sock);
}

void UpdateRecipients(char *update){ //Already using opened sockets
	Parse update...
	primaryRecipient = update[0];
	secondaryRecipient = update[1];

	UpdateRecipientsMsg response = new UpdateRecipientsMsg();

	while(sendto(svr_sock, response, strlen(response), 0, (struct sockaddr_in*)&svr_addr, sizeof(svr_addr))){};
}

void NotifyRecipientDied(char *peer){

}

String hashMessage(char *message)

//Send to Server s2p
void setupSendToServerSocket(){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  addr.sin_family = AF_INET
  addr.sin_port = htons(11111);
  inet_aton(g_szServer, &addr.sin_addr.s_addr);
  connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
}

//Send to Peers p2p current to next
void setupSendToPeersServer(){
  int sock1 = socket(AF_INET, SOCK_STREAM, 0);
  int sock2 = socket(AF,INET, SOCK_STREAM, 0);
  addr1.sin_family = AF_INET
  addr1.sin_port = htons(11111);
  addr2.sin_family = AF_INET
  addr2.sin_port = htons(11111);
  inet_aton(primaryRecipient, &addr1.sin_addr.s_addr);
  connect(sock1, (struct sockaddr_in*)&addr1,sizeof(addr1));
  inet_aton(secondaryRecipient, &addr2.sin_addr.s_addr);
  connect(sock2, (struct sockaddr_in*)&addr2,sizeof(addr2));
}
//Receive from Peers p2p previous to current
void setupReceiveFromPeersSocket(){
  int peer_sock = socket(AF_INET, SOCK_STREAM, 0);
  peer_addr.sin_family = AF_INET;
  peer_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  peer_addr.sin_port = htons(11111);


  bind(peer_sock, (struct sockaddr_in*)&peer_addr, sizeof(peer_addr));
  listen(peer_sock, BUFSIZE);

}
//Receive From Server //s2p
void setupReceiveFromServerSocket(){
  int cli_sock = socket(AF_INET, SOCK_STREAM, 0);
  cli_addr.sin_family = AF_INET;
  inet_aton(g_szServer, &cli_addr.sin_addr.s_addr);
  cli_addr.sin_port = htons(11111);


  bind(cli_sock, (struct sockaddr_in*)&cli_addr, sizeof(cli_addr));
  listen(cli_sock, BUFSIZE);
}