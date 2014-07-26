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
        std::string username;

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

        /*
        *Using the SHA256 Hash, any message recieved/sent will be hashed.
        */
        std::string hash256Message(TextMsg message);


        /*
         * Updates Recipients received from server
         */
        void updateRecipients(std::string one, std::string two);

        /*
        *Forces lowercase on Strings
        */
        std::string toLowerCase(std::string code);
        /*
        *Notifies the server if one of the two recipients of the peer went offline
        *and timed out the ACK response
        */
        void notifyRecipientDied(IPaddr recipient);

        /*
        *Sends a prompt of instructions to stdout
        */
        void Peer::printPrompt();
        
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
        *Send UI commands to server
        */
        void sendToServer();
};

#endif // PEER.H
