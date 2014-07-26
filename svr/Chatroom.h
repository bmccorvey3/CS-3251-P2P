#ifndef CHATROOM_H
#define CHATROOM_H

#include "Peer.h"

class Chatroom
{
    public:
        Chatroom(std::string name);
        ~Chatroom();
        std::string getName();

        /**
        * Update the appropriate peer's username
        */
        void updatePeerUsername(IPaddrStruct* IPaddr, std::string& username);

        /**
        * Remove a particular peer from the list
        * TODO updatePeers()?
        */
        Peer* removePeer(IPaddrStruct& stIPaddr);

        /**
        * Remove a peer and return the index of the first peer to be updated
        */
        int removePeer(IPaddrStruct* IPaddr);

        /**
        * Get the structure that identifies which for which peers I should update recipients
        */
        UpdateRecipientStruct* Chatroom::getUpdateStruct(int indexRemoved);

        /**
        * Get number of peers in the chatroom
        */
        int getSize();

        /**
         * Calculate the modulus of a num, but make sure it's positive
         * NOTE: only works for values where you have to add or subtract
         * the mod once.
         */
        int positiveMod(int num, int mod);

    protected:
    private:
        std::list<Peer> m_peers;
        std::string m_name; // alphanumeric, max of 16 characters
};

#endif // CHATROOM_H
