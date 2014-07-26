//  Created by Andrew Scheinbach on 7/20/14.
//  Copyright (c) 2014 Andrew Scheinbach. All rights reserved.

#ifndef PEER_H
#define PEER_H

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

#include "sha256.h"

#define BUFSIZE 2048
#define serverPort 11111
#define primaryPort 22222
#define secondaryPort 33333



class Peer
{
    public:
        Peer(std::string server);
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
        *Vector of messages sent/recieved that are hashed
        */
        
        std::vector<std::string> m_messageHashes;
        /*
        *Queue that stores any messages unable to be sent to the 2 recipients
        */
        std::queue<std::string> m_messageList;

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
        IPAddr primaryRecipient;
        IPAddr secondaryRecipient;

        /*
        *Using the SHA256 Hash, any message recieved/sent will be hashed.
        */
        std::string hashMessage(char *message);

        /*
        *Forces lowercase on Strings
        */
        std::string toLowerCase();
        /*
        *Notifies the server if one of the two recipients of the peer went offline
        *and timed out the ACK response
        */
        void notifyRecipientDied();

        /*
        *Sends a prompt of instructions to stdout
        */
        void Peer::printPrompt();
        
        /*
        *Receive messages from previous peers
        */
        void receieveFromPeer(int portno);

        /*
        *Receive updates from Server
        */
        void receiveFromServer();

        /*
        *Send messages to Peer; Linearly:PrimaryRecipient, SecondaryRecipient
        */
        void sendToPeer();

        /*
        *Send UI commands to server
        */
        void sendToServer();


}