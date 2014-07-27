//  Created by Andrew Scheinbach on 7/20/14.
//  Copyright (c) 2014 Andrew Scheinbach. All rights reserved.

#include "peer.h"

Peer::Peer(char *server)
{
	std::string serverName(server);

	std::queue<std::string> m_messageList; //messageMutex
	std::vector<std::string> m_messageHashes; //hashMutex

	std::string m_username = NULL;
	std::string m_myChatroom = NULL;

	IPaddr primaryRecipient; //recipientMutex
	IPaddr secondaryRecipient;

	static pthread_mutex_t messageMutex; //Access by receivieFromPeers,sendToServer,sendToPeers
	static pthread_cond_t messagesToSend; //receiveFromPeers adds to Vector

	static pthread_mutex_t hashMutex; //Access by receivefromPeers,sendToServer

	static pthread_mutex_t recipientMutex; //Access by receiveFromServer,sendToPeers

	pthread_mutex_init(&messageMutex, NULL);
	pthread_cond_init(&messagesToSend, NULL);
}

Peer::~Peer()
{
	delete std::string serverName; //Use std::string serverName(server) to get std::string of server

	delete std::queue<TextMsg> m_messageList;
	delete std::vector<std::string> m_messageHashes;

	delete std::string m_username;

	delete IPaddr primaryRecipient;
	delete IPaddr secondaryRecipient;


}

void Peer::mutexInit(){
	pthread_mutex_init(&messageMutex, NULL);
	pthread_cond_init(&messagesToSend, NULL);

	pthread_mutex_init(&hashMutex, NULL);

	pthread_mutex_init(&recipientMutex, NULL);
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
  TextMsg *msg = TextMsg::getInstance(buffer);
  string hashed = hash256Message(msg);
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
	  pthread_mutex_lock(&messageMutex);
	  pthread_mutex_lock(&hashMutex);
  	m_messageHashes.push_back(hashed);
  	  pthread_mutex_unlock(&hashMutex);
  	m_messageList.push(*msg);
  	  pthread_cond_broadcast(&messagesToSend);
  	  pthread_mutex_unlock(&messageMutex);
  	cout << msg->getTextPayload();
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
	  BaseMessage *msg = BaseMessage::getInstance(buffer);
  	  int type = msg->getMessageType(buffer);
  	  //UpdateRecipients Message
  	  if(type == UPDATE_S2P){
    		UpdateRecipientsMsg *update = UpdateRecipientsMsg::getInstance(buffer);
    		string newPrimary = update->getPrimaryRecipients();
    		string newSecondary = update->getSecondaryRecipients();

    		sockaddr_in *new1 =update->getIPaddr1();
    		sockaddr_in *new2 =update->getIPaddr2();

    		updateRecipients(new1, new2);
    		cout << newPrimary << newSecondary;

    		response = (void*) msg->getMessageStruct();
    		while(send(svr_sock, response, BUFSIZE,0));
    	}
  	//Peer Dropped Message
    	if(type == NOTIFY_S2P){
    		NotifyDroppedPeerMsg *update = NotifyDroppedPeerMsg::getInstance(buffer);
    		cout << update->getPayloadString();
    	}
    	else{
    		cout << "Received invalid server message";
    	}
  }
	close(this_sock);
	close(svr_sock);
}

void Peer::sendToPeers(){
	using namespace std;

	void buffer[BUFSIZE];
	void buffer2[BUFSIZE];

	/********************************************************
	*PrimaryRecipient
	********************************************************/
	//Timeout Structure
	struct timeval timeout;
  	timeout.tv_sec = 120;
  	timeout.tv_usec = 0;

	//Create Socket on port portno 22222 to send to PrimaryRecipient IPaddr
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
  	addr.sin_port = htons(primaryPort);
  	addr.sin_addr = primaryRecipient;

  	//inet_aton(recipient, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));

  	//Setsockopt for Timeouts

  	bool timed = false;
  	if(setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout))<0){
  	}

  	/********************************************************
	*SecondaryRecipient
	********************************************************/
	//Timeout Struct
	struct timeval timeout2;
	timeout2.tv_sec = 120;
	timeout2.tv_usec == 0;

	//Create Socket on port portno 33333 to send to SecondaryRecipient IPaddr
	int sock2 = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr2;
	addr2.sin_family = AF_INET;
	addr2.sin_port = htons(secondaryPort);
	addr2.sin_addr = secondaryRecipient;
	//inet_aton(recipient, &addr.sin_addr.s_addr);
	connect(sock,(struct sockaddr*)&addr2,sizeof(addr2));
	memset(buffer2, '\0', sizeof(buffer2));

	//setsockopt for timeout
	bool timed2 = false;
	if(setsockopt(sock2, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout2, sizeof(timeout2))<0){

	}

  	//Send all messages in message Queue
	pthread_mutex_lock(&messageMutex);
	pthread_cond_wait(&messagesToSend, &messageMutex);
  	while(m_messageList.size > 0){

  		TextMsg txtMsg = m_messageList.pop();
  		buffer = txtMsg.getMessageStruct();
  		buffer2 = txtMsg.getMessageStruct();
  		int bytesRecv1=0;
  		while(bytesRecv1+=send(sock,buffer, BUFSIZE, 0)){
  			if(errno == EPIPE){
  				timed = true;
  				break;
  			}
  		}
  			int bytesRecv2=0;
			while(bytesRecv2+=send(sock,buffer2, BUFSIZE, 0)){
				if(errno == EPIPE){
				  timed2=true;
				  break;
				}
			}
  		if(bytesRecv1 == 0){
  			timed = true;
  		}
  		if(bytesRecv2 == 0){
  			timed2 = true;
  		}
  	}
	pthread_mutex_unlock(&messageMutex);

  	close(sock);
  	if(timed){
  		notifyRecipientDied(&primaryRecipient);
  	}
  	close(sock);
  	if(timed2){
  		notifyRecipientDied(&secondaryRecipient);
  	}


  	//Send all messages in message Queue
}
int Peer::createSocketToServer(){
	using namespace std;

		//Create Socket on port 11111 to send UI commands to Server
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in addr;
	  	addr.sin_family = AF_INET;
	 	addr.sin_port = htons(serverPort);
	 	const char *server = serverName.c_str();
	  	inet_aton(server, &addr.sin_addr);
	  	connect(sock, (struct sockaddr*)&addr,sizeof(addr));
	  	return socket;
}

void Peer::enter(std::string message){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));


	int sock = createSocketToServer();
	EnterChatroomMsg outputMsg = EnterChatroomMsg(m_username, Direction::P2S, message);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	EnterChatroomMsg chat = response;
	std::cout << chat.getPayloadString();
	close(sock);
 	m_myChatroom = message;
}

void Peer::leave(){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	memset(buffer, '\0', sizeof(buffer));
	memset(response, '\0', sizeof(response));

	int sock = createSocketToServer();
	LeaveChatroomMsg outputMsg = LeaveChatroomMsg(m_username, Direction::P2S, m_myChatroom);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	LeaveChatroomMsg left = response;
	std::cout << left.getPayloadString();
	close(sock);
 	m_myChatroom = NULL;
}

void Peer::create(std::string chatroomName){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	memset(buffer, '\0', sizeof(buffer));
	memset(response, '\0', sizeof(response));

	int sock = createSocketToServer();
	CreateChatroomMsg outputMsg = CreateChatroomMsg(m_username, Direction::P2S, chatroomName);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	CreateChatroomMsg created = response;
	std::cout << created.getPayloadString();
	close(sock);
}

void Peer::destroy(std::string chatroomName){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	memset(buffer, '\0', sizeof(buffer));
	memset(response, '\0', sizeof(response));

	int sock = createSocketToServer();
	DestroyChatroomMsg outputMsg = DestroyChatroomMsg(m_username, Direction::P2S, chatroomName);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	DestroyChatroomMsg destroyed = response;
	std::cout << destroyed.getPayloadString();
	close(sock);
}

void Peer::user(std::string message){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	memset(buffer, '\0', sizeof(buffer));
	memset(response, '\0', sizeof(response));

	int sock = createSocketToServer();
	ChooseUsernameMsg outputMsg = ChooseUsernameMsg(message, Direction::P2S);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	ChooseUsernameMsg user = response;
	std::cout <<user.getPayloadString();
	m_username = user.getUsername();
	close(sock);
}

void Peer::list(){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	memset(buffer, '\0', sizeof(buffer));
	memset(response, '\0', sizeof(response));

	int sock = createSocketToServer();
	ListChatroomMsg outputMsg = ListChatroomMsg(Direction::P2S, NULL);
	buffer = outputMsg.getMessageStruct();
	while(send(sock,buffer, BUFSIZE, 0)){};

	int numBytesReceived = 0;
	while(numBytesReceived += recv(sock, response+numBytesReceived, BUFSIZE, 0)){};

	ListChatroomMsg list = response;
	std::cout << list.getPayloadString();
	close(sock);
}

void Peer::text(std::string message){
	TextMsg textMessage = new TextMsg(m_username, message, NULL);
	std::string hashed = hash256Message(&textMessage);
	pthread_mutex_lock(&messageMutex);
	pthread_mutex_lock(&hashMutex);
	m_messageHashes.push_back(hashed);
	pthread_mutex_unlock(&hashMutex);
	m_messageList.push(textMessage);
	pthread_cond_broadcast(&messagesToSend);
	pthread_mutex_unlock(&messageMutex);

}

void Peer::notifyRecipientDied(IPaddr *recipient){
	void buffer[BUFSIZE];
	void response[BUFSIZE];
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));


	//Create Socket on port 11111 to send UI commands to Server
	int sock = createSocketToServer();

	NotifyDroppedPeerMsg outputMsg = NotifyDroppedPeerMsg(m_username, Direction::P2S, m_myChatroom, &recipient);
  	buffer = outputMsg.getMessageStruct();
  	while(send(sock,buffer, BUFSIZE, 0)){};

  	close(sock);
}

void Peer::operateUI(){
	using namespace std;

  	string command;
  	cin >> command;
  	char *commandChar = command.c_str();

  	string code = strtok(commandChar, " ");
  	string message = strtok(NULL, " ");

  	code = toLowerCase(code);

  	if(code.compare("text:")){
  		text(message);
	}
	else if(code.compare("enter:")){
		enter(message);
	}
	else if(code.compare("leave:")){
		leave();
	}
	else if(code.compare("create:")){
		create(message);
	}
	else if(code.compare("destroy:")){
		destroy(message);
	}
	else if(code.compare("list:")){
		list();
  	}
	else if(code.compare("user:")){
		user(message);
	}	
	else if(code.compare("h")){
		printPrompt();
	}
	else{
		std::cout << "Invalid UI command, press h to see available commands";
	}
}

void Peer::updateRecipients(sockaddr_in *one, sockaddr_in *two){
	pthread_mutex_lock(&recipientMutex);
	primaryRecipient = one->sin_addr;
	secondaryRecipient = two->sin_addr;
	pthread_mutex_unlock(&recipientMutex);
}

void Peer::printPrompt(){
 std::cout <<"Press h for help"
 <<"Available Commands:" <<std::endl
 <<"Text: Writes Text to others in Chatroom" <<std::endl
 <<"Enter: 'Chatroom': Enters chatroom of specified name" <<std::endl
 <<"Leave: Leaves current chatroom" <<std::endl
 <<"Create: 'Chatroom': Creates chatroom of specified name" <<std::endl
 <<"Destroy: 'Chatroom': Destroys chatroom of specified name" <<std::endl
 <<"List: Lists available chatrooms to join" <<std::endl
 <<"User: 'username': Creates username specified (Required before chatting)";
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

std::string Peer::hash256Message(TextMsg *message){
	unsigned char *output;
	const unsigned char *msg = message->getPayloadString();
	//SHA256().CalculateDigest(pbOutputBuffer, pbData, nDataLen);
	CryptoPP::SHA256::SHA256().CalculateDigest(output, msg, sizeof(message));
	std::string returnMessage(output);

	return returnMessage;
}




