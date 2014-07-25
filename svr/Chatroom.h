#ifndef CHATROOM_H
#define CHATROOM_H

#include "Peers.h"

class Chatroom
{
    public:
        Chatroom(std::string name);
        virtual ~Chatroom();
        std::string toString();
        Peers peers,
        std::string name, // alphanumeric, max of 16 characters

        /**
        * Based on the peer that was most recently changed, update appropriate peers
        * TODO account for mutex
        */
        void updatePeers();

        /**
        * Remove a particular peer from the list
        * TODO updatePeers()?
        */
        Peer* removePeer(stIPaddr& stIPaddr);
    protected:
    private:
        std::list<Peer> m_peers;
};

#endif // CHATROOM_H
