#include "Server.h"
#include "BaseMessage.h"
#define PROGRAM_TYPE "Server"

Server::Server() {
	unsigned short tempPort = LISTENING_PORT;
	m_listeningPort = htons(tempPort);
}

Server::~Server() {
}

int main(int argc, char argv[]) {
	Server server = new Server();
	server.monitor();
}

void Server::monitor() {
	printf("Starting server\n");

	// Open a socket
	int svr_sock = socket(AF_INET, SOCK_STREAM, 0);

	// declare address from which I should receive and therefore listen for
	struct sockaddr_in svr_addr;
	svr_addr.sin_family = AF_INET; // still using IP
	// I want to accept a connection from any IP address
	svr_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	svr_addr.sin_port = m_listeningPort; // I want to accept from port 11111, and I already called htonl

	// I want to be known as the the IP address
	//		and port number specified in the parameters
	if (bind(svr_sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
#ifdef DEBUG_SERVER
		// Notify if there's an error
		fprintf(stderr, "Bind failed in %s\n\tError: %s\n", PROGRAM_TYPE,
				strerror(errno));
#endif
		return;
	}
	listen(svr_sock, SIZE_OF_BACKLOG_QUEUE); // Now I'm accepting connections

	// Attend to clients
	char buffer[BUFFER_SIZE];
	int cli_sock = 0;
	while (1)   // always be on the lookout for clients
	{
		memset(&buffer, 0, sizeof(buffer)); // clear the buffer
		bool foundRequestEnd = false;
		//do {
			int numBytesRecv = 0;
			// accept any client
			cli_sock = accept(svr_sock, NULL, NULL);
			if (cli_sock == -1) {
#ifdef DEBUG_SERVER
				fprintf(stderr, "Error in accepting client in %s\n",
				PROGRAM_TYPE);
#endif
			}
			// receive data from the client
			numBytesRecv += recv(cli_sock, &buffer[numBytesRecv],
					sizeof(buffer), 0); // added +numBytesReceived
#ifdef DEBUG_SERVER
			printf("Now server has received %d bytes\n", numBytesRecv);
#endif
			// if we successfully received data, print the buffer
			if (numBytesRecv > 0) {
#ifdef DEBUG_SERVER
				printf("\"%s\"\n", buffer);
#endif
				// we got to the end of the request
				foundRequestEnd = true; // break out at the end of the loop
				BaseMessage* msg = BaseMessage::getInstance(buffer);
				respondToRequest(msg, cli_sock);
//                } // if receivedFullPacket
			} // if numBytesRecv > 0
			else {
				// if you couldn't receive, close the connection
				close(cli_sock);
			}
		//} while (!foundRequestEnd);
		// at this point, you may loop around to attend to other clients
	} // while(1)
	close(svr_sock); // close the server when you're completely done
}

void Server::respondToRequest(BaseMessage* msg, int cli_sock) {
	switch (msg->getMessageType()) {
	case CHOOSE_P2S:
		chooseUsername((ChooseUsernameMsg::ChooseUsernameMsg*) msg, cli_sock);
		break;
	case NOTIFY_P2S:
		close(cli_sock); // close the connection; open new ones later
		updateRecipients((NotifyDroppedPeerMsg::NotifyDroppedPeerMsg*) msg,
				cli_sock);
		break;
	case LIST_P2S:
		listChatrooms((ListChatroomMsg::ListChatroomMsg*) msg, cli_sock);
		break;
	case CREATE_P2S:
		createChatroom((CreateChatroomMsg::CreateChatroomMsg*) msg, cli_sock);
		break;
	case DESTROY_P2S:
		destroyChatroom((DestroyChatroomMsg::DestroyChatroomMsg*) msg,
				cli_sock);
		break;
	case LEAVE_P2S:
		leaveChatroom((LeaveChatroomMsg::LeaveChatroomMsg*) msg, cli_sock);
		break;
	case ENTER_P2S:
		enterChatroom((EnterChatroomMsg::EnterChatroomMsg*) msg, cli_sock);
		break;
	default:
		fprintf(stderr,
				"Msg %s wasn't recognized in Server::respondToRequest\n",
				BaseMessage::log(msg));
		close(cli_sock); // close the connection
		break;
	}
}

void Server::sendMessage(BaseMessage* msg, int cli_sock) {

	unsigned int bytesToSend = 0;
	unsigned int bytesSent = 0;
	do {
		void* response = (void*) msg->getMessageStruct();
		bytesToSend = msg->getLength();
		// send the message as long as there are more bytes to send
		bytesSent += send(cli_sock, &response[bytesSent],
				bytesToSend - bytesSent, 0);
#ifdef DEBUG_SERVER
		fprintf(stderr, "Have now sent %d bytes of %d from %s\n", bytesSent,
				bytesToSend,
				PROGRAM_TYPE);
#endif
	} while (bytesSent < bytesToSend);
	close(sock);
}

void Server::sendMessage(BaseMessage* msg, IPaddrStruct* ipAddr) {
	// Open a socket using TCP
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	// Make sure socket was created
	if (sock == -1)
	{
		fprintf(stderr, "Error opening socket in %s\n", PROGRAM_TYPE);
		return;
	}

	// Define the address
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr)); // zero it out
	addr.sin_family = AF_INET; // sin = source in
	addr.sin_port = m_listeningPort;

	// copy address to addr struct
	memcpy(&addr.sin_addr, &ipAddr->sin_addr, sizeof(ipAddr->sin_addr));

	// connect to the server
	if (-1 == connect(sock, (const struct sockaddr*) &addr, sizeof(addr)))
	{
		fprintf(stderr, "Couldn't connect to socket in %s\n\tError: %s\n",
				PROGRAM_TYPE, strerror(errno));
		return;
	}
	// Now that the socket is open, send the message
	sendMessage(msg, sock);
}

void Server::dump() {
	// TODO
}

void Server::chooseUsername(ChooseUsernameMsg::ChooseUsernameMsg* msg,
		int cli_sock) {

	// Associate IP address with a username
	IPaddrStruct* ipAddr = getIPaddrStructFromSocket(cli_sock);
	// Add that peer to the list of floating peers
	if (msg->getChatRoomName() == NULL) // not yet in a chatroom-> update m_floatingPeers
	{
		Peer peer = new Peer(ipAddr, msg->getUsername());
		m_floatingPeers.push_back(peer);

	} else { // already in a chatroom-> update chatroom's peer structure
		Chatroom* curChatroom = getChatroom(msg->getChatRoomName());
		std::string tempUsername = msg->getUsername();
		curChatroom->updatePeerUsername(ipAddr, tempUsername);
	}
	// send confirmation message
	ChooseUsernameMsg confirmMsg = ChooseUsernameMsg(msg->getUsername(), Direction::S2P);
	sendMessage((BaseMessage*)&confirmMsg, cli_sock);
}

void Server::updateRecipients(NotifyDroppedPeerMsg::NotifyDroppedPeerMsg* msg,
		int cli_sock) {
	// for the appropriate chatroom
	Chatroom* curChatroom = getChatroom(msg->getChatRoomName());
	// update the peers affected by most recent change
	IPaddrStruct* ipAddrDropped = msg->getDroppedPeerIPaddr();
	// notify dropped peer
	int indexRemoved = curChatroom->removePeer(ipAddrDropped);
	UpdateRecipientStruct* updateRecipientStruct = curChatroom->getUpdateStruct(
			indexRemoved);
	UpdateRecipientsMsg firstMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName(),
			updateRecipientStruct->firstPeerSecondaryRecipient, updateRecipientStruct->firstPeerSecondaryRecipient);
	sendMessage(&firstMsg, updateRecipientStruct->firstPeerToUpdate);
	UpdateRecipientsMsg secondMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName(),
			updateRecipientStruct->secondPeerPrimaryRecipient,updateRecipientStruct->secondPeerSecondaryRecipient);
	sendMessage(&firstMsg, updateRecipientStruct->secondPeerToUpdate);

}

void Server::listChatrooms(ListChatroomMsg::ListChatroomMsg* msg,
		int cli_sock) {
	// for each of the chatrooms,
	std::ostringstream output;
	bool requiresComma = false;
	for (std::list<Chatroom>::iterator it = m_chatrooms.begin();
			it != m_chatrooms.end(); ++it) {
		if (requiresComma) {
			output << ",";
		}
		requiresComma = true;
		output << it->getName();
	}
	ListChatroomMsg retMsg = ListChatroomMsg(Direction::S2P, output.str());
	sendMessage((BaseMessage*)&retMsg, cli_sock);
}

void Server::createChatroom(CreateChatroomMsg::CreateChatroomMsg* msg,
		int cli_sock) {
	CreateChatroomMsg retMsg;
	//if chatroom doesn’t yet exist,
	if (getChatroom(msg->getChatRoomName()) != nullptr) {
		//Add to list of chatrooms // new Chatroom;
		Chatroom newChatroom = new Chatroom(new std::string(msg->getChatRoomName()));
		m_chatrooms.push_back(newChatroom);
		// send confirmation
		retMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName());
	} else {
		retMsg(msg->getUsername(), Direction::ERROR, msg->getChatRoomName());
	}
	sendMessage((BaseMessage*)&retMsg, cli_sock);
}

void Server::destroyChatroom(DestroyChatroomMsg::DestroyChatroomMsg* msg,
	int cli_sock) {
	std::string msgChatroomName = msg->getChatRoomName();
	DestroyChatroomMsg retMsg;
	for (std::list<Chatroom>::iterator it = m_chatrooms.begin();
			it != m_chatrooms.end(); ++it) {
		if (it->getName().compare(msgChatroomName) == 0) {
			if (it->getSize() > 0) { //If there are people in the chatroom
				// send forbidden message
				retMsg(msg->getUsername(), Direction::ERROR, msg->getChatRoomName());
			} else {
				m_chatrooms.erase(it);
				// send confirmation
				retMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName());
			}
		}
	}
	sendMessage((BaseMessage*)&retMsg, cli_sock);
}

void Server::leaveChatroom(LeaveChatroomMsg::LeaveChatroomMsg* msg,
	int cli_sock) {
	// For the appropriate chatroom
	Chatroom* chatroom = getChatroom(msg->getChatRoomName());
	// remove the peer from the list --> move into list of floating peers
	Peer* peer = chatroom->removePeer(getIPaddrStructFromSocket(cli_sock));
	m_floatingPeers.push_back(*peer);
	LeaveChatroomMsg retMsg = LeaveChatroomMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName());
	sendMessage((BaseMessage*)&retMsg, cli_sock);
}

void Server::enterChatroom(EnterChatroomMsg::EnterChatroomMsg* msg,
	int cli_sock) {
	EnterChatroomMsg retMsg;
	Peer* peerToMove = nullptr;
	for (std::list<Peer>::iterator it = m_floatingPeers.begin();
			it != m_floatingPeers.end(); ++it) {
		if (it->getIPaddr()->sin_addr.s_addr == getIPaddrStructFromSocket(cli_sock)->sin_addr.s_addr) {
			peerToMove = it;
			m_floatingPeers.erase(it);
		}
	}
	if (peerToMove == nullptr) {
		// If peer is not in floating peers, send forbidden message
		retMsg = EnterChatroomMsg(msg->getUsername(), Direction::ERROR, msg->getChatRoomName());
	} else {
		retMsg = EnterChatroomMsg(msg->getUsername(), Direction::S2P, msg->getChatRoomName());
	}

	// For the appropriate chatroom
	Chatroom* chatroom = getChatroom(msg->getChatRoomName());
	// remove the peer from the list --> move into list of floating peers
	Peer* peer = chatroom->removePeer(getIPaddrStructFromSocket(cli_sock));
	// chatroom->removePeer();
	m_floatingPeers.push_back(*peer);
	sendMessage((BaseMessage*)&retMsg, cli_sock);
}

// TODO make sure this address gets freed eventually
IPaddrStruct* Server::getIPaddrStructFromSocket(int cli_sock) {
// see http://beej.us/guide/bgnet/output/html/multipage/getpeernameman.html
struct sockaddr_storage addr;
// char ipstr[INET6_ADDRSTRLEN];
// int port;

socklen_t len = sizeof addr;
getpeername(cli_sock, (struct sockaddr*) &addr, &len);

// deal with both IPv4 and IPv6:
if (addr.ss_family == AF_INET) {
	IPaddrStruct *tempExtractedIPaddrStruc = (IPaddrStruct *) &addr;
	// port = ntohs(s->sin_port);
	// inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	IPaddrStruct retIPaddr = memcpy(malloc(sizeof(IPaddrStruct)),
			tempExtractedIPaddrStruc, sizeof(IPaddrStruct));
	return retIPaddr;
} else { // IPv6
	fprintf(stderr, "Rejecting IPv6 address");
	return nullptr;
}
}

Chatroom* Server::getChatroom(const std::string& chatroomName) {
for (std::list<Chatroom>::iterator it = m_chatrooms.begin();
		it != m_chatrooms.end(); ++it) {
	if (it->getName().compare(chatroomName) == 0) {
		return it;
	}
}
// if we got this far, there is no such chatroom
return nullptr;
}
