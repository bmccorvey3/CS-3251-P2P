CC = g++
DEBUG_FLAGS = -g -O0 -DDEBUG
CPPFLAGS = $(DEBUG_FLAGS) -Wall
RM = rm -f

all: peer_driver server_driver

msgs: ./msg/msgBaseMessage.o ./msg/ChooseUsernameMsg.o ./msg/CreateChatroomMsg.o ./msg/DestroyChatroomMsg.o\
	./msg/EnterChatroomMsg.o ./msg/LeaveChatroomMsg.o ./msg/ListChatroomMsg.o ./msg/NotifyDroppedPeerMsg.o\
	./msg/TextMsg.o ./msg/UpdateRecipientsMsg.o

peer_driver: ./peer/peer.o ./peer/peerDriver.o msgs
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

server_driver: ./svr/Chatroom.o ./svr/List.o ./svr/Peer.o ./svr/Server.o msgs
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) *.o peer_driver msgs server_driver
