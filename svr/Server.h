#ifndef SERVER_H
#define SERVER_H

#include "Chatrooms.h"


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
        dump();

    protected:
    private:
        /**************************************************
        * Data
        ***************************************************/
        /**
        * Doubly linked list of chatrooms
        */
        Chatrooms m_chatrooms;
        /**
        * Circular, doubly linked list of peers
        */
        Peers m_floatingPeers;

        /**************************************************
        * Functions
        ***************************************************/

        /*
        * Associate IP address with a username and add to list of floating peers
        */
        void setPeerUsername();

        /*
        * If someone drops out, update the affected peers
        * Try to tell peer that dropped out “You have been knocked off.”
        */
        void updatePeers();

        /*
        * Send back a list of chatrooms
        */
        void listChatrooms();

        /*
        * Create chatroom unless name is already taken
        */
        void createChatroom();

        /*
        * Destroy chatroom if empty or else warn that destruction is forbidden.
        */
        void destroyChatroom();

        /*
        * Remove peer from chatroom list, put in list of floating peers
        */
        void exitChatroom();

};

#endif // SERVER_H
