#ifndef PEER_H
#define PEER_H

class Peer
{
	public:
		Peer();
		~Peer();

		 typedef struct sockaddr_in IPaddr_struct;
            struct sockaddr_in {
                short            sin_family;   // e.g. AF_INET, AF_INET6
                unsigned short   sin_port;     // e.g. htons(3490)
                struct in_addr   sin_addr;     // see struct in_addr, below
                char             sin_zero[8];  // zero this if you want to
            };

        /*
        * The actual IP address
        */
        typedef struct in_addr IPaddr;
            struct in_addr {
                unsigned long s_addr; // load with inet_pton()
            };

		/****
		*Functions
		****/

	protected:
	private:
		/*
		*Queue of messages sent/recieved that are hashed
		*/
		queue m_messageHashes;
		/*
		*Queue that stores any messages unable to be sent to the 2 recipients
		*/
		queue m_messageList;

		char *string username;
		char *string portNumber;
		char *string ServerName;

		IPAddr primaryRecipient;
		IPAddr secondaryRecipient;

		static p_thread_mutex_t sendRecieveMutex;

		/*
		*Creation of Peer, taking in server 
		*/
		int main(int argc, char **argv);


		/*
        * Sends message to server
        */
        void send();

        /*
        * Receive message from server, print to stdout
        */
        void receive();

        /*
        * changes the first and second recipients based on the server structure
        * in the current chatroom
        */
        void updateRecipients(char *payload);

        /*
        *Using the SHA256 Hash, any message recieved/sent will be hashed.
        */
        String hashMessage(char *message);

        /*
        *Updates the peer's username and sends it to the server
        */
        void createUsername(char *payload);

        /*
        *Lets the server know the peer is leaving the chatroom
        */
        void leaveChatroom();

        /*
        *Enters selected Chatroom
        */
        void enterChatroom(char *payload);

        /*
        *Ask to join current chatroom if it isn't at max capacity
        */
        void joinChatroom(char *payload);

        /*
        *Requests to destroy current chatroom in the server
        */
        void destroyChatroom(char *payload);

        /*
        * Tells server the peer is going offline
        */
        void goOffline();

        /*
        *Requests that a chatroom be created by the server
        */
        void createChatroom();

        /*
        *Notifies the server if one of the two recipients of the peer went offline
        *and timed out the ACK response
        */
        void notifyRecipientDied();

        /*
        *Sets up TCP Socket
        */

        //Send Peer to Server commands
        void setupSendToServerSocket();
        //Sends messages forward to next peers
        void setupSendToPeersServer();
        //Receive messages from previous peers
        void setupReceiveFromPeersSocket();
        //Receive updates from Server
        void setupReceiveFromServerSocket();


}