//
//  BaseMessage.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__BaseMessage__
#define __MessageHierarchyP2P__BaseMessage__

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <cstdlib>
#define USERNAME_LENGTH 16
#define CHATROOM_NAME_LENGTH 16
#define CODE_LENGTH 4

using std::string;

enum class Direction {
    P2S,
    S2P,
    P2P,
    ERROR
};

typedef enum MESSAGE_TYPE {
    TEXT,
    LEAVE_P2S,
    LEAVE_S2P,
    ENTER_P2S,
    ENTER_S2P,
    ERR_ENTER_S2P,
    DESTROY_P2S,
    DESTROY_S2P,
    ERR_DESTROY_S2P,
    CREATE_P2S,
    CREATE_S2P,
    ERR_CREATE_S2P,
    LIST_P2S,
    LIST_S2P,
    NOTIFY_P2S,
    NOTIFY_S2P,
    ERR_NOTIFY_S2P,
    UPDATE_P2S,
    UPDATE_S2P,
    CHOOSE_P2S,
    CHOOSE_S2P,
    MT_ERROR
} MessageType;

//struct message_t {
//    unsigned int length;
//    char username[16];
//    unsigned int salt;
//    char type[4];
//    char chatroom[16];
//    char payload[1024];
//} StBaseMessage;

typedef struct __attribute__((packed)) header_t {
    unsigned int length;
    char username[16];
    unsigned int salt;
    char code[4];
    char chatRoomName[16];
} StBaseHeader;

class BaseMessage {
public:
    //BaseMessage(); // TODO add m_salt = rand();
    BaseMessage(void* parseMe);
    BaseMessage(string username, Direction dir, string chatRoomName);
    
    /*
     Static constant for the length of header
     */
    
    static const unsigned int HEADER_LENGTH = sizeof(unsigned int) +
                            sizeof(char[16]) + sizeof(unsigned int) +
                            sizeof(char[4]) + sizeof(char[16]);
    
    static MessageType convertStringToMessageType(std::string stringType, Direction &direction);
    virtual ~BaseMessage() {
        //TODO add destructor to delete pointers
    }
    
    StBaseHeader* getHeaderStruct();
    
//    virtual void setUsername(string);
//    virtual void setSalt(unsigned int);
//    virtual void setMessageType(MessageType);
//    virtual void setStringType(string);
//    virtual void setDirection(Direction);
//    virtual void setChatroom(string);
//    virtual void setPayload(string);
    
    unsigned int getLength();
    string getUsername();
    //unsigned int getSalt(); Don't need to get the salt
    static MessageType getMessageType(void* input);
    MessageType getMessageType();
    string getMessageCode();
    Direction getDirection();
    string getChatRoomName();
    void* getPayloadPtr(); //Make sure these are void pointers
    //void* getPayloadPtr();
    
    // Get struct to output to socket
    virtual void* getMessageStruct() = 0;
    // Print out debug info
    static string log(BaseMessage*);
    // Make sure 2 messages are equal
    static void assertEquals(BaseMessage* left, BaseMessage* right);
    // Get message that can be cast to another kind of message from a void*
    static BaseMessage* getInstance(void*);
    
    virtual string getPayloadString() = 0;
    
   // static virtual BaseMessage* getParticularMessage() = 0;
    
//protected:

    unsigned int m_length;
    string m_username;
    unsigned int m_salt;
    MessageType m_messageType;
    string m_code;
    Direction m_dir;
    string m_chatRoomName;
    void* m_payload;
    string m_payloadString;
};

#endif /* defined(__MessageHierarchyP2P__BaseMessage__) */
