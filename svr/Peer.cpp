#include "Peer.h"
#include <list>

Peer::Peer(IPaddr_struct* stIPaddr, std::string username)
{
    m_IPaddr = *stIPaddr;
    m_username = new std::string(username);
}

Peer::~Peer()
{
    free(m_IPaddr);
    delete(m_username);
}

std::string Peer::getUsername()
{
    return m_username;
}

IPaddrStrut Peer::getIPaddr()
{
    return m_IPaddr;
}

void Peer::setUsername(std::string username)
{
    m_username = username;
}

//void Peer::addPeer(Peer &peer)
//{
//
//}

//void Peer::addPeer(IPaddr_struct &stIPaddr, std::string username)
//{
//    Peer peer = createPeer(stIPaddr, username);
//
//}

//Peer* Peer::createPeer()
//{
//
//}

//void Peer::freePeer(void* peer)
//{
//    delete(peer->username);
//    free(peer);
//}

//void Peer::populatePeerQuartet(void* peer)
//{
//    // TODO ensure mutex
//    // populate first pair
//    // populate second pair
//}

//Peer::PeerPair Peer::populatePeerPair(void* peer)
//{
//    peer = (Peer*) peer; // cast as a peer
//    // When you find the peer you're looking for,
//    m_whichPeerPair = ppFirst;
//    if (peer == ppFirst)
//    {
//        // put current peer in m_curPeerPair.first
//        m_curPeerPair.primary = peer;
//        m_whichPeerPair = ppSecond;
//    }
//    else if (peer == ppSecond)
//    {
//        // put current peer as second peer
//        m_curPeerPair.secondary = peer;
//    }
//    // TODO test, put in cases for if sending peer is last or second-to-last peer
//}

//int Peer::shouldRemovePeer(const void * peer)
//{
//    // if the peer is the peer we're looking for,
//    // remove it
//}

//void Peer::removePeer(IPaddr_struct &stPeerIPaddr)
//{
//    // TODO ensure mutex
//
//}

//void Peer::removePeer(Peer &peer)
//{
//    IPaddr_struct stPeerIPaddr = peer.ip;
//    removePeer(stPeerIPaddr);
//}
