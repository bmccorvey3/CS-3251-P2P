#include "Chatroom.h"

// TODO implement

Chatroom::Chatroom(std::string name)
{
    m_name = new std::string(name);
}

Chatroom::~Chatroom()
{
    //dtor
}

std::string Chatroom::getName()
{
    return m_name;
}

void Chatroom::updatePeerUsername(IPaddrStruct* ipAddr, std::string& username)
{
    for (std::list<Peer>::iterator it=m_peers.begin(); it != m_peers.end(); ++it)
    {
    	// if this is the username we're looking for
        if (it->getIPaddr()->sin_addr.s_addr == ipAddr->sin_addr.s_addr){
            it->setUsername(username);
            return;
        }
    }
    #ifdef DEBUG_SERVER
    fprintf(stderr, "Couldn't set username %s'", username);
    #endif // DEBUG_SERVER
}

int Chatroom::removePeer(IPaddrStruct* ipAddr)
{
    int indexRemoved = 0;
    for (std::list<Peer>::iterator it=m_peers.begin(); it != m_peers.end(); ++it)
    {
        if (it->getIPaddr()->sin_addr.s_addr == ipAddr){
            m_peers.erase(it);
            return indexRemoved;
        } else {
            indexRemoved++;
        }
    }
	#ifdef DEBUG_SERVER
	fprintf(stderr, "Couldn't remove peer at IPaddr %d", ipAddr->sin_addr.s_addr);
	#endif // DEBUG_SERVER
	return -1;
}

UpdateRecipientStruct* Chatroom::getUpdateStruct(int indexRemoved)
{
    UpdateRecipientStruct* newRecipients = malloc(sizeof(UpdateRecipientStruct));
    memset(newRecipients, 0, sizeof(UpdateRecipientStruct));
    // newRecipients->firstPeerPrimaryRecipient;
    // -2--> -1, 0
    // -1--> 0, 1
    int indexArr[4];
    IPaddrStruct* IPs[4];
    int length = (int) m_peers.size();
    // calculate the indexes of the values of which I'm interested
    for (int i = -2; i < 2; i++){
        int peerIndex = positiveMod(i,length);
        indexArr[i+2] = peerIndex;
    }

    // iterate through the peers to get appropriate IPaddresses
    int curPeerInd = 0;
    for (std::list<Peer>::iterator it=m_peers.begin(); it != m_peers.end(); ++it)
    {
        for (int j = 0; j < 4; j++)
        {
            if (indexArr[j] == curPeerInd){
                IPs[j] = it->IPaddr;
            }
        }
        curPeerInd++;
    }

    // populate struct to return
    newRecipients->firstPeerToUpdate = IPs[0];
    newRecipients->firstPeerPrimaryRecipient = IPs[1];
    newRecipients->firstPeerSecondaryRecipient = IPs[2];
    newRecipients->secondPeerToUpdate = IPs[1];
    newRecipients->secondPeerPrimaryRecipient = IPs[2];
    newRecipients->secondPeerSecondaryRecipient = IPs[3];
    return newRecipients;
}

int Chatroom::positiveMod(int num, int mod)
{
//    int len = 6;
//	int removed = 0;
//	int offset = -1;
//	int num = removed + offset;
	if (num < 0){
		num += mod;
	} else if (num > mod-1){
		num -=mod;
	}
	return num;
}

int Chatroom::getSize()
{
    return m_peers.size();
}
