#ifndef PEERS_H
#define PEERS_H


class Peer
{
    public:
        Peer(IPaddrStruct IPaddr, std::string username);
        ~Peer();
        //Able to handle removal from the middle

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

        /**
        * Represents a peer with username and IP address
        */
//        typedef struct PEER {
//            IPaddr_struct ip;
//            std::string username;
//
//            /// Full disclosure: I'm not checking username for equality;
//            /// only IP address and port number
//            bool operator==(const Peer& p) const
//            {
//                return (ip.sin_addr.in_addr == p.ip.sin_addr.in_addr)
//                    &&  (ip.sin_port == p.ip.sin_port);
//            }
//        } Peer;

        /**
        * Returns true if peers are equal
        */
        bool equals(Peer);


//        /**
//        * The two peers to which a sending peer would send
//        */
//        typedef struct PEER_PAIR {
//            Peer* primary;
//            Peer* secondary;
//        } PeerPair;
//
//        /**
//        * The two sets of receiving peers to which 2 sending peers should send (respectively)
//        */
//        typedef struct PEER_QUARTET
//        {
//            PeerPair* first;
//            PeerPair* second;
//        } PeerQuartet;
//
//        typedef enum WHICH_PEER_PAIR
//        {
//            ppFirst = 1,
//            ppSecond = 2
//        } WhichPeerPair;
//
//        typedef enum WHICH_RECEIVER
//        {
//            prPrimary = 1,
//            prSecondary = 2
//        } WhichPeerPair;
//
//        /**
//        * Prepares to call getPeerQuartet() by filling in the peers for a the peer
//        * stored in m_sendingPeer.
//        */
//        void populatePeerQuartet(void* peer);
//
//        /**
//        * Create and add a peer to the list of peers (in the back of the list)
//        */
//        void addPeer(IPaddr_struct &stIPaddr, std::string username);

    private:

//        std::vector<Peer>       m_peers;
//        IPaddr_struct           m_stPeerIPaddrToRemove;
//        PeerPair*               m_firstPeerPair;
//        PeerPair*               m_secondPeerPair;
//        PeerPair*               m_curPeerPair;
//        WhichPeerPair           m_whichPeerPair; // current peer pair I'm populating
//        WhichReceiver           m_whichReceiver; // current receiving peer I'm populating
        std::string             m_username;
        IPaddrStruct            m_IPaddr;


//        /**
//        * Add a peer to the list of peers (in the back of the list)
//        */
//        void addPeer(Peer &peer);

        /**
        * Create a peer with the given IP address and username
        * NOTE: Remember to free the peer when done!
        */
        Peer* createPeer(IPaddr_struct &stIPaddr, std::string username);

//        /**
//        * Free the peer's malloc'ed struct
//        * Fulfills typedef void (*list_op)(void*);
//        */
//        void freePeer(void* peer);
//
//        /**
//        * Returns the pair to which the given peer is supposed to send.
//        */
//        PeerPair populatePeerPair(void* peer);
//
//        /**
//        * Returns non-zero if the peer should be removed, 0 if not.
//        */
//        int shouldRemovePeer(const void * peer);
//
//        /**
//        * Given an IP, removes that peer from the list
//        */
//        void removePeer(IPaddr_struct &stPeerIPaddr);
//
//        /**
//        * Given a peer, removes that peer from the list
//        */
//        void removePeer(Peer &peer);

};

#endif // PEERS_H
