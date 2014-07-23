#include "Server.h"

Server::Server()
{
    m_chatrooms = new Chatrooms();
    m_floatingPeers = new Peers();
}

Server::~Server()
{
    delete m_chatrooms;
    delete m_floatingPeers;
}

void Server::monitor()
{
    // spawn threads
    while (1)
    {
         // wait for TCP messages
            // switch message type--> call corresponding method
                // set username
                // peer got knocked off
                // updatePeers
                // list chatrooms
                // create chatroom
                // destroy chatroom
                // exit chatroom
    }
}

void Server::dump()
{

}

void Server::setPeerUsername()
{
    // Associate IP address with a username
    // Add that peer to the list of floating peers
}

void Server::updatePeers()
{
    // for the appropriate chatroom
        // update the peers affected by most recent change
}

void Server::listChatrooms()
{
    // for each of the chatrooms,
        // get the toString
    // create the appropriate message and send the payload
}

void Server::createChatroom()
{
    //if chatroom doesn’t yet exist,
    //Add to list of chatrooms // new Chatroom;
    //else,
    //Send “forbidden” message
}

void Server::destroyChatroom()
{
    //If the chatroom isn’t empty,
    //Send forbidden message
    //Else
    //Destroy chatroom and remove it from list of chatrooms // delete Chatroom
}

void Server::exitChatroom()
{
    // For the appropriate chatroom
    // remove the peer from the list --> move into list of floating peers
    // chatroom->removePeer();
}
