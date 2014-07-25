//  Created by Andrew Scheinbach on 7/20/14.
//  Copyright (c) 2014 Andrew Scheinbach. All rights reserved.

#include "peer.h"

Peer::Peer(std::string server)
{
	void buffer[BUFSIZE];
	void response[BUFSIZE];
	std::string serverName = server;

	std::queue<std::string> m_messageList;
	std::vector<std::string> m_messageHashes;

	std::string username;

	IPAddr primaryRecipient;
	IPAddr secondaryRecipient;

	static p_thread_mutex_t sendRecieveMutex;
}

Peer::~Peer()
{
	delete void buffer[BUFSIZE];
	delete void response[BUFSIZE];
	delete char *server; //Use std::string serverName(server) to get std::string of server

	delete Queue<std::TextMsg> m_messageList;
	delete Vector<std::string> m_messageHashes;

	delete std::string username;

	delete IPAddr primaryRecipient;
	delete IPAddr secondaryRecipient;

}


void Peer::receiveFromPeer(int portno){
	using namespace std;
  //Creation of Socket for this Peer to Listen for Incoming Peer Messages
  struct sockaddr_in this_addr;
  int this_sock = socket(AF_INET, SOCK_STREAM, 0);
  this_addr.sin_family = AF_INET;
  this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  this_addr.sin_port = htons(portno);
  bind(this_sock, (struct sockaddr_in*)&this_addr, sizeof(this_addr));
  listen(this_sock, BUFSIZE);

  //Creation of Socket of Peers sending messages to this Peer
  struct sockaddr_in sender_addr; 
  socklen_t sender_len = sizeof(sender_addr);
  int sender_sock = accept(this_sock, (struct sockaddr_in*)&sender_addr, &sender_len);
  memset(buffer, '\0', sizeof(buffer));
  memset(response, '\0', sizeof(response));

  //Receiving from other peers\
  int numBytesReceived=0;
  while(numBytesReceived += recvfrom(sender_sock, buffer+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&sender_addr, &sender_len)>0){};
  TextMsg message = buffer;
  String hashed = hashMessage(message);
  bool newInstance = true;
  //Iterate to check if message has already been received
  for(int i = 0; i < m_messageHashes.size(); i++){
  	if(strncmp(m_messageHashes[i],hashed){
  		newInstance = false;
  		break;
  	}
  }
  //Message not in hashed vector
  if(newInstance){
  	m_messageHashes.add(hashed);
  	m_messageList.add(message);
  	cout << message.getMessage();
  }
  close(this_sock);
  close(sender_sock);
}

void Peer::receiveFromServer(){
	using namespace std;
  //Creation of Socket for this Peer to Listen for Server Updates
  struct sockaddr_in this_addr;
  int this_sock = socket(AF_INET, SOCK_STREAM, 0);
  this_addr.sin_family = AF_INET;
  this_addr.sin_addr.s_addr = htonl(serverName);
  this_addr.sin_port = htons(serverPort);
  bind(this_sock, (struct sockaddr_in*)&this_addr, sizeof(this_addr));
  listen(this_sock, BUFSIZE);

  while(1)
	  //Creation of Socket of Server Sending Data to this Peer
	  struct sockaddr_in svr_addr; 
	  socklen_t svr_len = sizeof(svr_addr);
	  int svr_sock = accept(this_sock, (struct sockaddr_in*)&svr_addr, &svr_len);
	  memset(buffer, '\0', sizeof(buffer));
  	  memset(response, '\0', sizeof(response));

	  //Receiving from Server
	  int numBytesRecieved = 0;
	  while(numBytesReceived += recvfrom(svr_sock, buffer+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	  BaseMsg msg = buffer;
  	  String type = buffer.getType();
  	  String update = "update";
  	  String dropped = "dropped";
  	  //UpdateRecipients Message
  	  if(strcmp(type, update)){
    		UpdateRecipientMsg update = buffer;
    		updateRecipients(update.getPrimaryRecipients(),update.getSecondaryRecipients());
    		cout << update.getMessage(); 

    		response = new UpdateRecipients();
    		while(sendto(svr_sock, response, BUFSIZE,(struct sockaddr_in*)&this_addr, sizeof(this_addr)));
    	}
  	//Peer Dropped Message
    	if(strcmp(type, dropped)){
    		count << "You have been dropped";
    	}
  }
	close(this_sock);
	close(svr_sock);
}

void Peer::sendtoPeer(){
	using namespace std;
	//Create Socket on port portno 22222 to send to PRimaryRecipient IPAddr
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
  	addr.sin_port = htons(primaryPort);
  	addr.sin_addr = PrimaryRecipient;
  	//inet_aton(recipient, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

  	bool timeout = true;
  	//Send all messages in message Queue
  	while(m_messageList.size > 0){
  		buffer = m_messageList.pop();
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){
  			timeout = false;
  		}
  	}
  	close(sock);
  	if(timeout){
  		NotifyRecipientDied(PrimaryRecipient);
  	}

  	/********************************************************
  	*SecondaryRecipient
  	********************************************************/
  	//Create Socket on port portno 33333 to send to SecondaryRecipient IPAddr
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	struct sockaddr_in addr;
  	addr.sin_family = AF_INET;
  	addr.sin_port = htons(secondaryPort);
  	addr.sin_addr = SecondaryRecipient;
  	//inet_aton(recipient, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

  	bool timeout = true;
  	//Send all messages in message Queue
  	while(m_messageList.size > 0){
  		buffer = m_messageList.pop();
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){
  			timeout = false;
  		}
  	}
  	close(sock);
  	if(timeout){
  		NotifyRecipientDied(SecondaryRecipient);
  	}
}

void Peer::sendtoServer(){
	using namespace std;
	//Create Socket on port 11111 to send UI commands to Server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	addr.sin_family = AF_INET;
 	addr.sin_port = htons(serverPort);
  	inet_aton(serverName, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));

  	string command;
  	cin >> command;

  	string code = strtok(buff, " ");
  	string message = strtok(NULL, " ");

  	code = lowerCase(code);

  	if(strncmp(code, "text")){
		Textmsg textMessage = new Textmsg(message);
		hashed = hashMessage(textMessage);
		m_messageHashes.push(hashed);
		m_messageList.push(textMessage);
	}
	else if(strncmp(code, "enter")){
		buffer = new EnterChatroomMsg(message);
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	  
	  	cout << (EnterChatroomMsg)response.getChatRoomFromPayload();
	}
	else if(strncmp(code, "leave")){
		buffer = new LeaveChatroomMsg();
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	  
  	  	count << (LeaveChatroomMsg)response.getChatRoomFromPayload();
	}
	else if(strncmp(code, "create")){
		buffer = new CreateChatroomMsg(message);
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	  
  	  	cout << (CreateChatroomMsg)response.getChatRoomFromPayloadS2P();
	}
	else if(strncmp(code, "destroy")){
		buffer = new DestroyChatroomMsg(message);
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	  
  	  	cout << (DestroyChatroomMsg)response.getDestroyedChatRoomS2P();
	}
	else if(strncmp(code, "list")){
		buffer = new ListChatroomMsg();
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	
	  	cout <<(ListChatroomMsg)response.getListofChatrooms();
  	}
	else if(strncmp(code, "username")){
		buffer = new ChooseUsernameMsg(message);
  		while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  		int numBytesRecieved = 0;
	  	while(numBytesReceived += recvfrom(sock, response+numBytesReceived, BUFSIZE, 0, (struct sockaddr_in*)&svr_addr, &svr_len)>0){};
  	 
  	 	cout <<(ChooseUsernameMsg)response.getUsernameS2P();
  	 	username = response.getUsernameS2P();
	}	
	else if(strncmp(code, "h")){
		printPrompt();
	}
	else{}
	close(sock);
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

std::string Peer::hashMessage(BaseMsg message){
	char *output;
	//SHA256().CalculateDigest(pbOutputBuffer, pbData, nDataLen);
	SHA256().CalculateDigest(output, message.getMessage(), sizeof(message));
	std::string returnMessage(output);

	return returnMessage;
}

void Peer::NotifyRecipientDied(IPAddr recipient){
using namespace std;
	//Create Socket on port 11111 to send UI commands to Server
	int sock = socket(AF_INET, SOCK_STREAM, 0);
  	addr.sin_family = AF_INET;
 	addr.sin_port = htons(serverPort);
  	inet_aton(serverName, &addr.sin_addr.s_addr);
  	connect(sock, (struct sockaddr_in*)&addr,sizeof(addr));
  	memset(buffer, '\0', sizeof(buffer));
  	memset(response, '\0', sizeof(response));


  	buffer = new NotifyDroppedPeerMsg(recipient);

  	while(sendto(sock,buffer, BUFSIZE, 0, (struct sockaddr_in*)&addr, sizeof(addr))){};	

  	close(sock);
}



