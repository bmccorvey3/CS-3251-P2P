CC = g++
DEBUG_FLAGS = -g -O0 -DDEBUG
CPPFLAGS = $(DEBUG_FLAGS) -Wall
RM = rm -f

all: peer_driver server_driver

msgs: BaseMessage.o ChooseUsernameMsg.o CreateChatroomMsg.o DestroyChatroomMsg.o\
	EnterChatroomMsg.o LeaveChatroomMsg.o ListChatroomMsg.o NotifyDroppedPeerMsg.o\
	TextMsg.o UpdateRecipientsMsg.o

peer_driver: web_client.o
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

server_driver: web_server.o
	$(CC) $(CPPFLAGS) $(LDFLAGS) $^ -o $@

clean:
	$(RM) *.o web_server web_client
