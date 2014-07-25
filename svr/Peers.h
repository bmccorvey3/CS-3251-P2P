#ifndef PEERS_H
#define PEERS_H


class Peers
{
    public:
        Peers();
        ~Peers();
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
        typedef struct PEER {
            IPaddr_struct ip;
            std::string username;

            /// Full disclosure: I'm not checking username for equality;
            /// only IP address and port number
            bool operator==(const Peer& p) const
            {
                return (ip.sin_addr.in_addr == p.ip.sin_addr.in_addr)
                    &&  (ip.sin_port == p.ip.sin_port);
            }
        } Peer;

        /**
        * The two peers to which a sending peer would send
        */
        typedef struct PEER_PAIR {
            Peer* primary;
            Peer* secondary;
        } PeerPair;

        /**
        * The two sets of receiving peers to which 2 sending peers should send (respectively)
        */
        typedef struct PEER_QUARTET
        {
            PeerPair* first;
            PeerPair* second;
        } PeerQuartet;

        typedef enum WHICH_PEER_PAIR
        {
            ppFirst = 1,
            ppSecond = 2
        } WhichPeerPair;

        typedef enum WHICH_RECEIVER
        {
            prPrimary = 1,
            prSecondary = 2
        } WhichPeerPair;

        /**
        * Prepares to call getPeerQuartet() by filling in the peers for a the peer
        * stored in m_sendingPeer.
        */
        void populatePeerQuartet(void* peer);

        /**
        * Create and add a peer to the list of peers (in the back of the list)
        */
        void addPeer(IPaddr_struct &stIPaddr, std::string username);

    private:

        list*           m_peerList;
        IPaddr_struct   m_stPeerIPaddrToRemove;
        PeerPair*       m_firstPeerPair;
        PeerPair*       m_secondPeerPair;
        PeerPair*       m_curPeerPair;
        WhichPeerPair   m_whichPeerPair; // current peer pair I'm populating
        WhichReceiver   m_whichReceiver; // current receiving peer I'm populating


        /**
        * Add a peer to the list of peers (in the back of the list)
        */
        void addPeer(Peer &peer);

        /**
        * Create a peer with the given IP address and username
        */
        Peer* createPeer(IPaddr_struct &stIPaddr, std::string username);

        /**
        * Free the peer's malloc'ed struct
        * Fulfills typedef void (*list_op)(void*);
        */
        void freePeer(void* peer);

        /**
        * Returns the pair to which the given peer is supposed to send.
        */
        PeerPair populatePeerPair(void* peer);

        /**
        * Returns non-zero if the peer should be removed, 0 if not.
        */
        int shouldRemovePeer(const void * peer);

        /**
        * Given an IP, removes that peer from the list
        */
        void removePeer(IPaddr_struct &stPeerIPaddr);

        /**
        * Given a peer, removes that peer from the list
        */
        void removePeer(Peer &peer);

    /* A function pointer type that points to a function that takes in a void* and returns nothing, call it list_op */
    typedef void (*list_op)(void*);
    /* A function pointer type that points to a function that takes in a const void* and returns an int, call it list_pred */
    typedef int (*list_pred)(const void*);
    // /* A function pointer type that points to a function that takes in a void* and returns a void*, call it list_cpy */
    // typedef void* (*list_cpy)(const void*);

    // /* Creating */
    // list* create_list(void);
    // list* copy_list(list* llist, list_cpy copy_func);

    // /* Adding */
    // void push_front(list* llist, void* data); // don't plan to use this
    // void push_back(list* llist, void* data);

    // /* Removing */
    // int remove_front(list* llist, list_op free_func);
    // int remove_back(list* llist, list_op free_func);
    // int remove_if(list* llist, list_pred pred_func, list_op free_func);

    /* Querying List */
    void* front(list* llist);
    void* back(list* llist);
    // int is_empty(list* llist);
    int size(list* llist);

//    /* Freeing */
//    void empty_list(list* llist, list_op free_func);

    /* Traversal */
    void traverse(list* llist, list_op do_func);
};

#endif // PEERS_H
