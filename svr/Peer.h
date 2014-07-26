#ifndef PEERS_H
#define PEERS_H

#define DEBUG_SERVER

/*
* IPaddr + port, etc.
*/
typedef struct sockaddr_in IPaddrStruct;
//    struct sockaddr_in {
//        short            sin_family;   // e.g. AF_INET, AF_INET6
//        unsigned short   sin_port;     // e.g. htons(3490)
//        struct in_addr   sin_addr;     // see struct in_addr, below
//        char             sin_zero[8];  // zero this if you want to
//    };

/*
* The actual IP address
*/
typedef struct in_addr IPaddr;
//    struct in_addr {
//        unsigned long s_addr; // load with inet_pton()
//    };

typedef struct UPDATE_RECIPIENT_STRUCT {
	IPaddrStruct* firstPeerToUpdate;
	IPaddrStruct* firstPeerPrimaryRecipient;
	IPaddrStruct* firstPeerSecondaryRecipient;
	IPaddrStruct* secondPeerToUpdate;
	IPaddrStruct* secondPeerPrimaryRecipient;
	IPaddrStruct* secondPeerSecondaryRecipient;
} UpdateRecipientStruct;

class Peer
{
    public:
        Peer(IPaddrStruct* IPaddr, std::string username);
        ~Peer();

        /**
        * Returns true if peers are equal
        */
        bool equals(Peer);

        std::string getUsername();

        IPaddrStruct* getIPaddr();

        void setUsername(std::string username);

    private:

        std::string             m_username;
        IPaddrStruct*           m_IPaddr;

        /**
        * Create a peer with the given IP address and username
        * NOTE: Remember to free the peer when done!
        */
        Peer* createPeer(IPaddrStruct* stIPaddr, std::string username);

};

#endif // PEERS_H
