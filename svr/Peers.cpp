#include "Peers.h"

Peers::Peers()
{
    m_peerList = create_list(void);
}

Peers::~Peers()
{
    empty_list(m_peerList);
    empty_list(m_peerList, list_op free_func);
}

void Peers::addPeer(Peer &peer)
{
    push_back(m_peerList, peer);
}

void Peers::addPeer(IPaddr_struct &stIPaddr, std::string username)
{
    Peer peer = createPeer(stIPaddr, username);
    push_back(m_peerList, peer);
}

Peer* Peers::createPeer(IPaddr_struct &stIPaddr, std::string username)
{
    Peer* peer = malloc(sizeof(Peer));
    peer->ip = stIPaddr;
    peer->username = username;
    return peer;
}

void Peers::freePeer(void* peer)
{
    free(peer);
}

void Peers::populatePeerQuartet(void* peer)
{
    // TODO ensure mutex
    // populate first pair
    // populate second pair
}

Peers::PeerPair Peers::populatePeerPair(void* peer)
{
    peer = (Peer*) peer; // cast as a peer
    // When you find the peer you're looking for,
    m_whichPeerPair = ppFirst;
    if (peer == ppFirst)
    {
        // put current peer in m_curPeerPair.first
        m_curPeerPair.primary = peer;
        m_whichPeerPair = ppSecond;
    }
    else if (peer == ppSecond)
    {
        // put current peer as second peer
        m_curPeerPair.secondary = peer;
    }
    // TODO test, put in cases for if sending peer is last or second-to-last peer
}

int Peers::shouldRemovePeer(const void * peer)
{
    // if the peer is the peer we're looking for,
    // remove it
}

void Peers::removePeer(IPaddr_struct &stPeerIPaddr)
{
    // TODO ensure mutex
    m_stPeerIPaddrToRemove = stPeerIPaddr;
    int numPeersRemoved = remove_if(m_peerList, shouldRemovePeer, freePeer);
    if (numPeersRemoved != 1)
    {
        fprintf(stderr, "Removed %d peers when trying to remove 1.\n", numPeersRemoved);
    }
}

void Peers::removePeer(Peer &peer)
{
    IPaddr_struct stPeerIPaddr = peer.ip;
    removePeer(stPeerIPaddr);
}
