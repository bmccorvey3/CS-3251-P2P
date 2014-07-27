#include "Peer.h"
#include <list>

Peer::Peer(IPaddrStruct* stIPaddr, std::string username)
{
    m_IPaddr = stIPaddr;
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

IPaddrStruct* Peer::getIPaddr()
{
    return m_IPaddr;
}

void Peer::setUsername(std::string username)
{
    m_username = username;
}
