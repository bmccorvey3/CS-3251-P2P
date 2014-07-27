#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fstream>
#include <time.h>
#include <unistd.h> // necessary?
#include <sstream>
#include <iostream>
#include <cstring>
#define SIZE_OF_BACKLOG_QUEUE 10
#define PROGRAM_TYPE "Web Server"
#define BUFFER_SIZE 2048
#define DEFAULT_HTTP_PORT 80
#define DEBUG_SERVER // TODO remove this
#define TIME_BUFFER_SIZE 128
// #define ACTUAL_PATH_INDEX 2 // get past the "./"


#include "Chatroom.h"
#define LISTENING_PORT 11111

class Server
{
    public:
        Server();
        ~Server();

        /**************************************************
        * Functions
        ***************************************************/

        /**
        * Wait to receive messages
        */
        void monitor();

        /**
        * Dump all data to stdout
        */
        void dump();

        /**
        * Given a client socket, return the IPaddrStruct associated with it.
        */
        static IPaddrStruct* getIPaddrStructFromSocket(int cli_sock);

    protected:
    private:
        /**************************************************
        * Data
        ***************************************************/
        /**
        * Doubly linked list of chatrooms
        */
        std::list<Chatroom> m_chatrooms;
        /**
        * Circular, doubly linked list of peers
        */
        std::list<Peer> m_floatingPeers;

        /// The port on which we're listening for incoming connections
        const unsigned short m_listeningPort;

        /**************************************************
        * Functions
        ***************************************************/

        /**
        * Associate IP address with a username and add to list of floating peers
        */
        void chooseUsername(ChooseUsernameMsg::ChooseUsernameMsg* msg, int cli_sock);

        /**
        * If someone drops out, update the affected peers
        * Try to tell peer that dropped out “You have been knocked off.”
        */
        void updateRecipients(NotifyDroppedPeerMsg::NotifyDroppedPeerMsg* msg, int cli_sock);

        /**
        * Send back a list of chatrooms
        */
        void listChatrooms(ListChatroomMsg::ListChatroomMsg* msg, int cli_sock);

        /**
        * Create chatroom unless name is already taken
        */
        void createChatroom(CreateChatroomMsg::CreateChatroomMsg* msg, int cli_sock);

        /**
        * Destroy chatroom if empty or else warn that destruction is forbidden.
        */
        void destroyChatroom(DestroyChatroomMsg::DestroyChatroomMsg* msg, int cli_sock);

        /**
        * Remove peer from chatroom list, put in list of floating peers
        */
        void leaveChatroom(LeaveChatroomMsg::LeaveChatroomMsg* msg, int cli_sock);

        /**
         * Enter the chatroom
         */
        void enterChatroom(EnterChatroomMsg::EnterChatroomMsg* msg, int cli_sock)

        /**
         * Send a message
         */
        void sendMessage(BaseMessage* msg, int cli_sock);

        /**
		 * Connect to a client, then send the message
		 */
		void Server::sendMessage(BaseMessage* msg, IPaddrStruct* ipAddr);

        /**
         * Parse the chatroom for the given name from the list
         */
        Chatroom* getChatroom(const std::string& chatroomName);

        /**
         * Respond to the request of the client
         */
        void respondToRequest(BaseMessage* msg, int cli_sock)
};

#endif // SERVER_H
