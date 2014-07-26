//  Created by Andrew Scheinbach on 7/20/14.
//  Copyright (c) 2014 Andrew Scheinbach. All rights reserved.

#include "peer.h"

Peer::Peer(char *server)
{
	std::string serverName = server;

	std::queue<std::string> m_messageList;
	std::vector<std::string> m_messageHashes;

	std::string username;

	IPaddr primaryRecipient;
	IPaddr secondaryRecipient;

	static pthread_mutex_t sendRecieveMutex;
}

Peer::~Peer()
{
	delete std::string serverName; //Use std::string serverName(server) to get std::string of server

	delete std::queue<TextMsg> m_messageList;
	delete std::vector<std::string> m_messageHashes;

	delete std::string username;

	delete IPaddr primaryRecipient;
	delete IPaddr secondaryRecipient;

}


void Peer::receiveFromPeers(int portno){
	using namespace std;
	void buffer[BUFSIZE];
	void response[BUFSIZE];
  //Creation of Socket for this Peer to Listen for Incoming Peer Messages
  struct sockaddr_in this_addr;
  int this_sock = socket(AF_INET, SOCK_STREAM, 0);
  this_addr.sin_family = AF_INET;
  this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  this_addr.sin_port = htons(portno);
  bind(this_sock, (struct sockaddr*)&this_addr, sizeof(this_addr));
  listen(this_sock, BUFSIZE);

  //Creation of Socket of Peers sending messages to this Peer
  struct sockaddr_in sender_addr; 
  socklen_t sender_len = sizeof(sender_addr);
  int sender_sock = accept(this_sock, (struct sockaddr*)&sender_addr, &sender_len);
  memset(buffer, '\0', sizeof(buffer));
  memset(response, '\0', sizeof(response));

  //Receiving from other peers
  int numBytesReceived=0;
  while(numBytesReceived += recvfrom(sender_sock, buffer+numBytesReceived, BUFSIZE, 0, (struct sockaddr*)&sender_addr, &sender_len)>0){};
  TextMsg message = buffer;
  string hashed = hash256Message(message);
  bool newInstance = true;
  //Iterate to check if message has already been received
  for(int i = 0; i < m_messageHashes.size(); i++){
  	if(m_messageHashes[i].compare(hashed)){
  		newInstance = false;
  		break;
  	}
  }
  //Message not in hashed vector
  if(newInstance){
  	m_messageHashes.push_back(hashed);
  	m_messageList.push(message);
  	cout << message.getPayloadText();
  }
  close(this_sock);
  close(sender_sock);
}

void Peer::receiveFromServer(){
	using namespace std;
	void buffer[BUFSIZE];
	void response[BUFSIZE];
  //Creation of Socket for this Peer to Listen for Server Updates
  struct sockaddr_in this_addr;
  int this_sock = socket(AF_INET, SOCK_STREAM, 0);
  this_addr.sin_family = AF_INET;
  const char *server = serverName.c_str();
  //this_addr.sin_addr.s_addr = htonl(server);
  inet_aton(server, &this_addr.sin_addr);
  this_addr.sin_port = htons(serverPort);
  bind(this_sock, (struct sockaddr*)&this_addr, sizeof(this_addr));
  listen(this_sock, BUFSIZE);

  while(1){
	  //Creation of Socket of Server Sending Data to this Peer
	  struct sockaddr_in svr_addr; 
	  socklen_t svr_len = sizeof(svr_addr);
	  int svr_sock = accept(this_sock, (struct sockaddr*)&svr_addr, &svr_len);
	  memset(buffer, '\0', sizeof(buffer));
  	  memset(response, '\0', sizeof(response));

	  //Receiving from Server
	  int numBytesReceived = 0;
	  while(numBytesReceived += recv(svr_sock, buffer+numBytesReceived, BUFSIZE, 0)>0){};
  	  BaseMessage msg = buffer;
  	  string type = msg.getType();
  	  //UpdateRecipients Message
  	  if(type.compare("update")){
    		UpdateRecipientsMsg update = buffer;
    		updateRecipients(update.getPrimaryRecipients(),update.getSecondaryRecipients());
    		cout << update.getPrimaryRecipients() << update.getSecondaryRecipients();

    		response = new UpdateRecipientsMsg("hi");
    		while(send(svr_sock, response, BUFSIZE,0));
    	}
  	//Peer Dropped Message
    	if(type.compare("dropped")){
    		cout << "You have been dropped";
    	}
  }
	close(this_sock);
	close(svr_sock);
}

void Peer::sendToPeers(){
	using namespace std;
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	//Timeout Structure
	struct timeval timeout;
  	timeout.tv_sec = 120;
  	timeout.tv_usec = 0;

	//Create Socket on port portno 22222 to send to PRimaryRecipient IPaddr
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
  	addr.sin_port = htons(primaryPort);
  	addr.sin_addr = primaryRecipient;

  	//inet_aton(recipient, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

  	//Setsockopt for Timeouts

  	bool timed = false;
  	if(setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout))<0){
  	}

  	//Send all messages in message Queue
  	while(m_messageList.size > 0){
  		buffer = m_messageList.pop();
  		while(int value = send(sock,buffer, BUFSIZE, 0)){
  			if(value == EPIPE){
  				timed = true;
  			}
  		}
  	}

  	close(sock);
  	if(timed){
  		notifyRecipientDied(primaryRecipient);
  	}

  	/********************************************************
  	*SecondaryRecipient
  	********************************************************/
  	//Timeout Struct
  	struct timeval timeout2;
  	timeout2.tv_sec = 120;
  	timeout2.tv_usec == 0;

  	//Create Socket on port portno 33333 to send to SecondaryRecipient IPaddr
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
  	addr.sin_port = htons(secondaryPort);
  	addr.sin_addr = secondaryRecipient;
  	//inet_aton(recipient, &addr.sin_addr.s_addr);
  	connect(sock,(struct sockaddr*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

	//setsockopt for timeout
	bool timed2 = false;
  	if(setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout2, sizeof(timeout2))<0){
  		
  	}

  	//Send all messages in message Queue
  	while(m_messageList.size > 0){
  		buffer = m_messageList.pop();
  		while(int value =send(sock,buffer, BUFSIZE, 0)){
  			if(value == EPIPE){
  				timed2=true;
  			}
  		}
  	}

  	close(sock);
  	if(timed2){
  		notifyRecipientDied(secondaryRecipient);
  	}
}

void Peer::sendToServer(){
	using namespace std;
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	//Create Socket on port 11111 to send UI commands to Server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
 	addr.sin_port = htons(serverPort);
 	const char *server = serverName.c_str();
  	inet_aton(server, &addr.sin_addr);
  	connect(sock, (struct sockaddr*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

  	string command;
  	cin >> command;
  	char *commandChar = command.c_str();

  	string code = strtok(commandChar, " ");
  	string message = strtok(NULL, " ");

  	code = toLowerCase(code);

  	if(code.compare("text")){
		TextMsg textMessage = new TextMsg(message);
		string hashed = hash256Message(textMessage);
		m_messageHashes.push_back(hashed);
		m_messageList.push(textMessage);
	}
	else if(code.compare("enter")){
		buffer = new EnterChatroomMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	  	EnterChatroomMsg chat = response;
	  	cout << chat.getChatRoomFromPayload();
	}
	else if(code.compare("leave")){
		buffer = new LeaveChatroomMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	  	LeaveChatroomMsg left = response;
  	  	cout << left.getChatRoomFromPayload();
	}
	else if(code.compare("create")){
		buffer = new CreateChatroomMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

  		CreateChatroomMsg created = response;
  	  	cout << created.getChatRoomFromPayloadS2P();
	}
	else if(code.compare("destroy")){
		buffer = new DestroyChatroomMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};
  	  
	  	DestroyChatroomMsg destroyed = response;
  	  	cout << destroyed.getDestroyedChatRoomS2P();
	}
	else if(code.compare("list")){
		buffer = new ListChatroomMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};
  	
	  	ListChatroomMsg list = response;
	  	cout << list.getListofChatrooms();
  	}
	else if(code.compare("username")){
		buffer = new ChooseUsernameMsg(message);
  		while(send(sock,buffer, BUFSIZE, 0)){};

  		int numBytesReceived = 0;
	  	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};
  	 
	  	ChooseUsernameMsg user = response;
  	 	cout <<user.getUsernameP2S();
  	 	username = user.getUsernameP2S();
	}	
	else if(code.compare("h")){
		printPrompt();
	}
	else{}
	close(sock);
}

void Peer::updateRecipients(std::string one, std::string two){
	char *oneChar = one.c_str();
	char *twoChar = two.c_str();
	inet_aton(oneChar, &primaryRecipient);
	inet_aton(twoChar, &secondaryRecipient);
}

void Peer::printPrompt(){
 std::cout <<"Press h for help"
 <<"Available Commands:" <<std::endl
 <<"Text: Writes Text to others in Chatroom" <<std::endl
 <<"Enter 'Chatroom': Enters chatroom of specified name" <<std::endl
 <<"Leave: Leaves current chatroom" <<std::endl
 <<"Create 'Chatroom': Creates chatroom of specified name" <<std::endl
 <<"Destroy 'Chatroom': Destroys chatroom of specified name" <<std::endl
 <<"List: Lists available chatrooms to join" <<std::endl
 <<"User 'username': Creates username specified (Required before chatting)";
}

std::string Peer::toLowerCase(std::string message){
	std::string code ="";
	int i = 0;
	char c;
	while (message[i]){
	    c=message[i];
	    code += (tolower(c));
	    i++;
  	}
  	return code;
}

std::string Peer::hash256Message(TextMsg message){
	char *output;
	//SHA256().CalculateDigest(pbOutputBuffer, pbData, nDataLen);
	CryptoPP::SHA256::SHA256().CalculateDigest(output, (const char*)message.getPayloadText(), sizeof(message));
	std::string returnMessage(output);

	return returnMessage;
}

void Peer::notifyRecipientDied(IPaddr recipient){
using namespace std;
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	//Create Socket on port 11111 to send UI commands to Server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
 	addr.sin_port = htons(serverPort);
 	const char *server = serverName.c_str();
 	inet_aton(server, &addr.sin_addr);
  	connect(sock, (struct sockaddr*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));


  	buffer = new NotifyDroppedPeerMsg(recipient);

  	while(send(sock,buffer, BUFSIZE, 0)){};

  	close(sock);
}



