The structure is broken up into 3 distinct classes:
The Peer class which creates commands/text messages,
the server which keeps track of all the peers and the chatrooms
and the payload, which is broken up into multiple messages.

PEER

The Peer is broken up into the main Peer class, as well as the PeerDriver.
The PeerDriver runs the multiple peer files that deal with TCP connections, all running in seperate threads.

The peer has a messageQueue to store all messages to send out, a hashVector so store all messages hashed to check agaisnt for duplications, a username, a chatroom, two peer recipient IPaddresses, and mutex locks. The peer hashes messages using the SHA256 hash function. The main functionality of the peer is to make TCP connections with the other peers in a chatroom and with the server

There is a connection from previous peers sending new text data to the current peer.
To make sure no duplicate messages are received, a hash of the message is stored.
To make sure two hashes aren't equal if the message sent is the same, each message payload has a salt(random number) associated with it. Then these messages are saved to a messageQueue, to be parsed through and popped out when connections from the current Peer are sent to other peers.

There is also a connection from the server sending update data to the current peer.
A connection to constantly listen to the server must be made, so that the peer can always be updated on whenever the current peers in the chatroom change(Either by a new one entering, one leaving, or a peer unexpectantly dropping). It will listen and receive an UpdateRecipientMsg, changing the two recipients to new ones. The server also sends a message notifying any peer if they were dropped by mistake, if they timed out, or if they unexpectantly left of their own accord.

Thirdly, there is the connection from the peer to the main server. Th peer needs to send commands to the server to actually do anything, such as entering/leaving a chatroom, creating/destroying chatrooms, ect.. These are all done in a UI method which takes in characters from STDIN, parses the code and decides what helper function to call. Messages meant to be text messages are also queued in the messageQueue if they are written to the UI

Finally, there is are connections from the current peer sending data to the two recipient peers saved to the peer locally.
From the messages queue'd from previous peers and the UI, this connection pops evry available message out of the queue and sends it to the two recipient peers.

The peer can make the server: Let the peer enter a chatroom, Let the peer leave a chatroom, Let the peer create a chatroom that doesn't already exist, Let the peer destroy an empty chatroom, Let the peer know a list of all available chatrooms, Let the peer create a username for itself, Let the peer tell the server if another peer dropped connection, and let the peer know all of the previous commands.


MESSAGE

The message hierarchy is how peer and server communicate. These messages can either be interpreted as void* input or as standard input already available to the peer. 

Examples on how to create messages:
BaseMessage(void* input)
BaseMessage(string username, Direction dir, string chatRoomName, etc.)

The various message formats are in each of the individual files. 
BaseMessage is the base class and all other classes are derived classes of this base class. 


SERVER

/*Talk about server if necessary*/



WHAT WE COMPLETED

We implemented all the required work but the difficult part was getting each class to mesh together. All of the logic from our RFC is present and everything theoretically should work, but unfortunately due to the time constraints and the already massive amount of work put in, we were unable to complete the project. Nevertheless, partial credit would be greatly appreciated. 

Everything compiles in Eclipse. We didn't test the MakeFile even though we made one. 


CHANGES FROM THE RFC

1. Changed message payload to char buffer of max_size = 1024 bytes
2. Chatroom is in header (max 16 chars)
3. Seperate Ports to listen to based on whether the message is from the Server or from either of the Two Recipients (Port 22222, 33333) or the Server (Port 11111) to avoid any overlap in socket listening.
