#include "Server.h"

Server::Server() : m_chatrooms, m_floatingPeers,
{
    unsigned short tempPort = LISTENING_PORT;
    m_listeningPort = htons(tempPort);
}

Server::~Server()
{
}

void Server::monitor()
{
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
    if (bind(svr_sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1)
    {
#ifdef DEBUG_SERVER
        // Notify if there's an error
        fprintf(stderr, "Bind failed in %s\n\tError: %s\n", PROGRAM_TYPE,
                strerror(errno));
#endif
        return -1;
    }
    listen(svr_sock, SIZE_OF_BACKLOG_QUEUE); // Now I'm accepting connections

// Attend to clients
    char buffer[BUFFER_SIZE];
    int cli_sock = 0;
    while (1)   // always be on the lookout for clients
    {
        memset(&buffer, 0, sizeof(buffer)); // clear the buffer
        bool foundRequestEnd = false;
        do
        {
            int numBytesRecv = 0;
            // accept any client
            cli_sock = accept(svr_sock, NULL, NULL);
            if (cli_sock == -1)
            {
#ifdef DEBUG_SERVER
                fprintf(stderr, "Error in accepting client in %s\n", PROGRAM_TYPE);
#endif
            }
            // receive data from the client
            numBytesRecv += recv(cli_sock, buffer, sizeof(buffer), 0);
#ifdef DEBUG_SERVER
            printf("Now server has received %d bytes\n", numBytesRecv);
#endif
            // if we successfully received data, print the buffer
            if (numBytesRecv > 0)
            {
#ifdef DEBUG_SERVER
                printf("\"%s\"\n", buffer);
#endif

                // TODO find whether we've received the whole packet or whether we should wait for more
                if (receivedFullPacket) // TODO set this variable
                {
                    // we got to the end of the request
                    foundRequestEnd = true; // break out at the end of the loop
                    // TODO copy buffer contents into a msg that's malloc'ed
                    msg = MsgFactory.makeMessage(buffer);
                    respondToRequest(msg, cli_sock);
                } // if receivedFullPacket
            } // if numBytesRecv > 0
        } while (!foundRequestEnd);
        // at this point, you may loop around to attend to other clients
    } // while(1)
    close(svr_sock); // close the server when you're completely done
}

void Server::listen()
{

}

void Server::respondToRequest(BaseMessage msg, int cli_sock)
{
    switch (msg.getType())
    {
        case CHOOSE_USERNAME:
            chooseUsername((ChooseUsernameMsg::ChooseUsernameMsg)msg, cli_sock);
            break;
        case UPDATE_RECIPIENTS:
            close(cli_sock); // close the connection; open new ones later
            updateRecipients((NotifyDroppedPeerMsg::NotifyDroppedPeerMsg)msg);
            break;
        case LIST_CHATROOMS:
            listChatrooms((ListChatroomMsg::ListChatroomMsg)msg, cli_sock);
            break;
        case CREATE_CHATROOM:
            createChatroom((CreateChatroomMsg::CreateChatroomMsg)msg, cli_sock);
            break;
        case DESTROY_CHATROOM:
            destroyChatroom((DestroyChatroomMsg::DestroyChatroomMsg)msg, cli_sock);
            break;
        case LEAVE_CHATROOM:
            leaveChatroom((LeaveChatroomMsg::LeaveChatroomMsg)msg, cli_sock);
            break;
        default:
            fprintf(stderr, "Msg %s wasn't recognized in Server::respondToRequest\n", BaseMessage.log(msg));
            close(cli_sock); // close the connection
            break;
    }
 }

 void Server::send(BaseMessage msg, int cli_sock)
 {
    unsigned int bytesToSend = 0;
    unsigned int bytesSent = 0;
    do
    {
        void* repsonse = (void*) msg.getMessageStruct(); // TODO make this a method
        bytesToSend = msg.getTotalLength(); // TODO make this a method
        // send the message as long as there are more bytes to send
        bytesSent += send(cli_sock, &response[bytesSent],
                          bytesToSend - bytesSent, 0);
#ifdef DEBUG_SERVER
        fprintf(stderr,"Have now sent %d bytes of %d from %s\n", bytesSent, bytesToSend,
               PROGRAM_TYPE);
#endif
    } while (bytesSent < bytesToSend);
 }

void Server::dump()
{

}

void Server::chooseUsername(ChooseUsernameMsg::ChooseUsernameMsg& msg, int cli_sock)
{
    // Associate IP address with a username
    IPaddrStruct* IPaddr = getIPaddrStructFromSocket(int cli_sock);
    // Add that peer to the list of floating peers
    if (msg.getChatroomName() == NULL) // not yet in a chatroom
    {
        Peer peer = new Peer(IPaddr, msg.getUsername());
        m_floatingPeers.push_back(peer);

    }
    // TODO
    curChatroom.updateUser(msg.getUsername)
}

void Server::updateRecipients(NotifyDroppedPeerMsg::NotifyDroppedPeerMsg& msg)
{
    // for the appropriate chatroom
    curChatroom = Chatrooms.getChatroom(msg.getChatroomName());
    // update the peers affected by most recent change
    // TODO mutex lock
    curChatroom.dropPeer(msg.getDroppedIpP2S(), msg.getDroppedPortP2S());

}

void Server::listChatrooms(ListChatroomMsg::ListChatroomMsg& msg, int cli_sock)
{
    // for each of the chatrooms,
        // get the toString
    // create the appropriate message and send the payload
}

void Server::createChatroom(CreateChatroomMsg::CreateChatroomMsg& msg, int cli_sock)
{
    //if chatroom doesn’t yet exist,
    if (getChatroom(msg.getChatroomToCreate()) != nullptr)
    {
        //Add to list of chatrooms // new Chatroom;
        m_chatrooms.push_back(new Chatroom(new std::string(msg.)))
    }
    //else,
    //Send “forbidden” message
}

void Server::destroyChatroom(DestroyChatroomMsg::DestroyChatroomMsg& msg, int cli_sock)
{
    getChatroom()
    //If the chatroom isn’t empty,
    //Send forbidden message
    //Else
    //Destroy chatroom and remove it from list of chatrooms // delete Chatroom
}

void Server::leaveChatroom(LeaveChatroomMsg::LeaveChatroomMsg& msg, int cli_sock)
{
    // For the appropriate chatroom
    Chatroom* chatroom = getChatroom(msg.getChatroomName);
    // remove the peer from the list --> move into list of floating peers
    Peer* peer = chatroom->removePeer(getIPaddrStructFromSocket(cli_sock));
    // chatroom->removePeer();
    m_floatingPeers.push_back(*peer);
}

// TODO make sure this address gets freed eventually
IPaddrStruct* Server::getIPaddrStructFromSocket(int cli_sock)
{
    // see http://beej.us/guide/bgnet/output/html/multipage/getpeernameman.html
    struct sockaddr_storage addr;
    // char ipstr[INET6_ADDRSTRLEN];
    // int port;

    socklen_t len = sizeof addr;
    getpeername(cli_sock, (struct sockaddr*)&addr, &len);

    // deal with both IPv4 and IPv6:
    if (addr.ss_family == AF_INET) {
        IPaddrStruct *tempExtractedIPaddrStruc = (IPaddrStruct *)&addr;
        // port = ntohs(s->sin_port);
        // inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
        IPaddrStruct retIPaddr = memcpy(malloc(sizeof(IPaddrStruct)),
                                        tempExtractedIPaddrStruc,
                                        sizeof(IPaddrStruct));
        return retIPaddr;
    } else { // IPv6
        fprintf(stderr, "Rejecting IPv6 address");
        return nullptr;
    }
}

Chatrooom::Chatroom* getChatroom(const std::string& chatroomName)
{
    for (std::list<Chatroom>::iterator it=m_chatrooms.begin(); it != m_chatrooms.end(); ++it)
    {
        if (strcmp(it->toString(), chatroomName)){
            return it;
        }
    }
    // if we got this far, there is no such chatroom
    return nullptr;
}
