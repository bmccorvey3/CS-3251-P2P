//  Created by Andrew Scheinbach on 7/20/14.
//  Copyright (c) 2014 Andrew Scheinbach. All rights reserved.

#ifndef PEER_H
#define PEER_H

#include <arpa/inet.h>
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
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <queue>
#include <vector>
#include <assert.h>
#include <signal.h>
#include <alloca.h>
#include <pthread.h>

#include "SHA/sha.h"

#define BUFSIZE 2048
#define serverPort 11111
#define primaryPort 22222
#define secondaryPort 33333



class Peer
{
    public:
        Peer(char *server);
        ~Peer();

         typedef struct sockaddr_in IPaddr_struct;
            /*struct sockaddr_in {
                short            sin_family;   // e.g. AF_INET, AF_INET6
                unsigned short   sin_port;     // e.g. htons(3490)
                struct in_addr   sin_addr;     // see struct in_addr, below
                char             sin_zero[8];  // zero this if you want to
            };*/

        /*
        * The actual IP address
        */
        typedef struct in_addr IPaddr;
            /*struct in_addr {
                unsigned long s_addr; // load with inet_pton()
            };*/

        /****
        *Functions
        ****/

    protected:
    private:
        /*
        *Vector of messages sent/received that are hashed
        */
        
        std::vector<std::string> m_messageHashes;
        /*
        *Queue that stores any messages unable to be sent to the 2 recipients
        */
        std::queue<TextMsg> m_messageList;

        /*
        *Username used to define peer in chatroom
        */
        std::string m_username;

        /*
         * My chatroom
         */
        std::string m_myChatroom

        /*
        *Server that the peers are connecting to,
        *sending commands to and receiving updates from
        */
        std::string serverName;

        /*
        *The two recipients each peer sends text messages to
        */
        IPaddr primaryRecipient;
        IPaddr secondaryRecipient;

        static pthread_mutex_t messageMutex; //Access by receivieFromPeers,sendToServer,sendToPeers
		static pthread_cond_t messagesToSend; //receiveFromPeers adds to Vector

		static pthread_mutex_t hashMutex; //Access by receivefromPeers,sendToServer

		static pthread_mutex_t recipientMutex; //Access by receiveFromServer,sendToPeers

        /*
        *Using the SHA256 Hash, any message recieved/sent will be hashed.
        */
        std::string hash256Message(TextMsg *message);

        /*
         * initializes mutexes
         */
         void mutexInit();

        /*
         * Updates Recipients received from server
         */
        void updateRecipients(sockaddr_in *one, sockaddr_in *two);

        /*
        *Forces lowercase on Strings
        */
        std::string toLowerCase(std::string code);
        /*
        *Notifies the server if one of the two recipients of the peer went offline
        *and timed out the ACK response
        */
        void notifyRecipientDied(IPaddr *recipient);

        /*
        *Sends a prompt of instructions to stdout
        */
        void printPrompt();

        /*
         * Creates a socket connecting to the server
         */
        int createSocketToServer();
        
        /*
        *Receive messages from previous peers
        */
        void receiveFromPeers(int portno);

        /*
        *Receive updates from Server
        */
        void receiveFromServer();

        /*
        *Send messages to Peer; Linearly:PrimaryRecipient, SecondaryRecipient
        */
        void sendToPeers();

        /*
        *Send UI commands from stdin
        */
        void operateUI();

        /*
         * Enters chatroom 'chatroomName'
         */
        void enter(std::string chatroomName);

        /*
         * Leaves curent chatroom
        */
        void leave();


        /*
         * Creates chatroom 'chatroomName'
         */
        void create(std::string chatroomName);


        /*
         * destroys chatroom 'chatroomName'
         */
        void destroy(std::string message);

        /*
         * Makes username 'user'
         */
        void user(std::string user);

        /*
         * Lists available chatrooms
        */
        void list();

        /*
         * Adds message to messageQueue
         */
        void text(std::string message);
};

#endif // PEER.H
