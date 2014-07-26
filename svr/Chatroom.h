#ifndef CHATROOM_H
#define CHATROOM_H

#include "Peers.h"

class Chatroom
{
    public:
        Chatroom(std::string name);
        ~Chatroom();
        std::string getName();

        typedef struct UPDATE_RECIPIENT_STRUCT {
            IPaddrStruct* firstPeerToUpdate;
            IPaddrStruct* firstPeerPrimaryRecipient;
            IPaddrStruct* firstPeerSecondaryRecipient;
            IPaddrStruct* secondPeerToUpdate;
            IPaddrStruct* secondPeerPrimaryRecipient;
            IPaddrStruct* secondPeerSecondaryRecipient;
        } UpdateRecipientStruct;

        /**
        * Update the appropriate peer's username
        */
        void updatePeerUsername(IPaddrStruct* IPaddr, std::string username);

        /**
        * Remove a particular peer from the list
        * TODO updatePeers()?
        */
        Peer* removePeer(IPaddrStruct& stIPaddr);

        /**
        * Remove a peer and return the index of the first peer to be updated
        */
        int dropPeer(IPaddrStruct* IPaddr);

        /**
        * Get the structure that identifies which for which peers I should update recipients
        */
        UpdateRecipientStruct* Chatroom::getUpdateStruct(indexRemoved);

        /**
        * Get number of peers in the chatroom
        */
        int getSize();

    protected:
    private:
        std::list<Peer> m_peers;
        std::string m_name, // alphanumeric, max of 16 characters
};

#endif // CHATROOM_H
