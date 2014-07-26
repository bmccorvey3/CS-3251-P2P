//
//  BaseMessage.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "BaseMessage.h"
#include "TextMsg.h"
#include "ChooseUsernameMsg.h"
#include "CreateChatroomMsg.h"
#include "DestroyChatroomMsg.h"
#include "EnterChatroomMsg.h"
#include "LeaveChatroomMsg.h"
#include "ListChatroomMsg.h"
#include "NotifyDroppedPeerMsg.h"
#include "UpdateRecipientsMsg.h"


/*

lvsp
 take in username
 
 direction constructor. put in direction enum. 
 based on direction, format into a payload
 
 take out length
 
 i create the salt. random number
 length, type, salt
 
 constructor for each message type
 
 username get from constructor
 
 i'm creating the payload
 
 change naming convention to m_
 
 cast binary data 
 
 parse message constructor
 
 chatroom name as arg in every message
 
 
 
 
 // send
 EnterChatroomMsg msg = new EnterChatroomMsg("Name of Chatroom", "username", P2S);
 TCPconnection.send((void*)msg.getStruct());
 
 // receive
 ChooseUsernameMsg* msg = ChooseUsernameMsg.parseMsg(TCP.getPayload());
 
 static ChooseUsernameMsg* ChooseUsernameMsg::parseMsg(void*)
{
	// parse out all args
    // pull out header
    // make sure it’s of type cups (or cusp)
    // get length
    // parse from remaining buffer the length indicated in header as payload (in this case, username and some other stuff)
	return new ChooseUsernameMsg(args);
}
 
 
*/

BaseMessage::BaseMessage(void* msg)
{
    /*malloc size of structure
     memcpy to the structure
     
     */
    
//    char* charMsg = (char*) msg;
//    char username[16];
//    char type[4];
//    char chatroom[16];
//    char payload[1024];
    // parse through message and construct a BaseMessage with all the member variables filled in
    // StBaseMessage* stMsg = static_cast<message_t*>(msg);
    // TODO make these constants somewhere--maybe as #define
    /*int indexIntoLength = 0;
    int indexIntoUsername = sizeof(unsigned int);
    int indexIntoSalt = indexIntoUsername + sizeof(char[16]);
    int indexIntoType = indexIntoSalt + sizeof(char[4]);
    int indexIntoChatroom = indexIntoType + sizeof(char[4]);
    int indexIntoPayload = indexIntoChatroom + sizeof(char[16]);*/
    // TODO others...
    
    // index to get out member variables
    

//    unsigned int length = (unsigned int) charMsg[BaseMessage::indexIntoLength];
//    memcpy(username, &charMsg[BaseMessage::indexIntoUsername], sizeof(username));
//    unsigned int salt = (unsigned int) charMsg[BaseMessage::indexIntoSalt];
//    memcpy(type, &charMsg[BaseMessage::indexIntoType], sizeof(type));
//    memcpy(chatroom, &charMsg[BaseMessage::indexIntoChatroom], sizeof(chatroom));
//    memcpy(payload, &charMsg[BaseMessage::indexIntoPayload], sizeof(payload));
    
    StBaseHeader* stMsg = (StBaseHeader*) msg;
    m_length = stMsg->length;
    m_username = stMsg->username;
    m_salt = stMsg->salt;
    m_code = stMsg->code;
    m_chatRoomName = stMsg->chatRoomName;
    
    std::string stringType = std::string(m_code);
    Direction direction = Direction::ERROR;
    MessageType msg_type = MessageType::MT_ERROR;
    
    
//    BaseMessage* outputMsg = new BaseMessage(username, salt, direction);
//    
//    outputMsg->setLength(length, payload); // TODO make function
//    m_length = 
//    outputMsg->setUsername(string(username));
//    outputMsg->setSalt(salt);
//    outputMsg->setMessageType(msg_type);
//    outputMsg->setStringType(string(type));
//    outputMsg->setDirection(direction);
//    outputMsg->setChatroom(string(chatroom));
//    outputMsg->setPayload(string(payload));
//    m_salt = rand();
    
   /* string length_msg = str_msg.substr(0,sizeof(header.length));
    string user_msg = str_msg.substr(4,sizeof(header.username));
    string salt_msg = str_msg.substr(20,sizeof(header.salt));
    string type_msg = str_msg.substr(24, sizeof(header.type)); */
    
//    return outputMsg;
    
}

// TODO check scope on this
MessageType BaseMessage::convertStringToMessageType(std::string stringType, Direction &direction){
    direction = Direction::ERROR;
    MessageType msg_type = MT_ERROR;
    if (strcmp(stringType.c_str(), "text") == 0){
        direction = Direction::P2P;
        msg_type = MessageType::TEXT;
    } else if (strcmp(stringType.c_str(), "lvps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::LEAVE_P2S;
    } else if (strcmp(stringType.c_str(), "lvsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::LEAVE_S2P;
    } else if (strcmp(stringType.c_str(), "enps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::ENTER_P2S;
    } else if (strcmp(stringType.c_str(), "ensp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::ENTER_S2P;
    } else if (strcmp(stringType.c_str(), "efsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::ERR_ENTER_S2P;
    } else if (strcmp(stringType.c_str(), "dsps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::DESTROY_P2S;
    } else if (strcmp(stringType.c_str(), "dssp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::DESTROY_S2P;
    } else if (strcmp(stringType.c_str(), "dnsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::ERR_DESTROY_S2P;
    } else if (strcmp(stringType.c_str(), "crps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::CREATE_P2S;
    } else if (strcmp(stringType.c_str(), "crsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::CREATE_S2P;
    } else if (strcmp(stringType.c_str(), "cfsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::ERR_CREATE_S2P;
    } else if (strcmp(stringType.c_str(), "lsps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::LIST_P2S;
    } else if (strcmp(stringType.c_str(), "lssp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::LIST_S2P;
    } else if (strcmp(stringType.c_str(), "pdps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::NOTIFY_P2S;
    } else if (strcmp(stringType.c_str(), "pdsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::NOTIFY_S2P;
    } else if (strcmp(stringType.c_str(), "pksp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::ERR_NOTIFY_S2P;
    } else if (strcmp(stringType.c_str(), "upps") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::UPDATE_P2S;
    } else if (strcmp(stringType.c_str(), "upsp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::UPDATE_S2P;
    } else if (strcmp(stringType.c_str(), "cups") == 0){
        direction = Direction::P2S;
        msg_type = MessageType::CHOOSE_P2S;
    } else if (strcmp(stringType.c_str(), "cusp") == 0){
        direction = Direction::S2P;
        msg_type = MessageType::CHOOSE_S2P;
    } else {
        fprintf(stderr, "Unknown message type\n");
    }
    return msg_type;

}

BaseMessage* BaseMessage::getInstance(void* input){
    Direction dir;
    MessageType type = getMessageType(input);
    StBaseHeader* header = (StBaseHeader*) input;
    unsigned int length = header->length;
    char* username = username = header->username;
    unsigned int salt = header->salt;
    char* code(header->code);
    char* chatRoomName = header->chatRoomName;
    
    switch (type) {
        case TEXT:
            return new TextMsg(input);
            break;
        case LEAVE_P2S:
        case LEAVE_S2P:
            return new LeaveChatroomMsg(input);
            break;
        default:
            break;
    }
}

message_t* BaseMessage::getMessageStruct() {
    message_t* retStruct = (message_t*)malloc(sizeof(HEADER_LENGTH) + sizeof(m_payload));
    StBaseMessage.length = length;
    
    
    
    memcpy(StBaseMessage.username, m_username.c_str(), sizeof(StBaseMessage.username));
    //Buffer username with null bytes
    if(strlen(StBaseMessage.username) < 16) {
        memset(StBaseMessage.username + strlen(m_username.c_str()), '0',
               16 - strlen(m_username.c_str()));
    }
    
    
    StBaseMessage.salt = m_salt;
    memcpy(StBaseMessage.type, m_string_type.c_str(), sizeof(StBaseMessage.type));
    
    memcpy(StBaseMessage.chatroom, m_chatRoomName.c_str(), sizeof(StBaseMessage.chatroom));
    //Buffer chatroom with null bytes
    if(strlen(StBaseMessage.chatroom) < 16) {
        memset(StBaseMessage.chatroom + strlen(m_chatRoomName.c_str()), '0',
               16 - strlen(m_chatRoomName.c_str()));
    }
    
    
    memcpy(StBaseMessage.payload, m_payload.c_str(), sizeof(StBaseMessage.payload));
    
    memcpy(retStruct, &StBaseMessage, sizeof(&retStruct));
    return retStruct;
    
}

static string log(BaseMessage& msg) {
    std::stringstream ss;
    ss << "The length is: " << msg.getLength() << std::endl;
    ss << "The username is: " << msg.getUsername() << std::endl;
    ss << "The salt is: " << msg.getSalt() << std::endl;
    ss << "The message type is: " << msg.getMessageType() << std::endl;
    ss << "The string type is: " << msg.getStringType() << std::endl;
    ss << "The direction is: " << static_cast<int>(msg.getDirection()) << std::endl;
    ss << "The chatroom name is: " << msg.getChatroom() << std::endl;
    ss << "The payload is: " << msg.getPayloadString() << std::endl;
    return ss.str();
}

//void BaseMessage::setLength(unsigned int l, char[]) {
//    m_length = l;
//    
//    //TODO didn't set length of total message
//}
//
//void BaseMessage::setUsername(string username) {
//    m_username = username;
//}
//
//void BaseMessage::setSalt(unsigned int salt) {
//    m_salt = salt;
//}
//
//void BaseMessage::setMessageType(MessageType msg) {
//    m_mess_type = msg;
//}
//
//void BaseMessage::setStringType(string type) {
//    m_string_type = type;
//}
//
//void BaseMessage::setDirection(Direction dir) {
//    m_dir = dir;
//}
//
//void BaseMessage::setChatroom(string chatroom) {
//    m_chatRoomName = chatroom;
//}
//
//void BaseMessage::setPayload(string payload) {
//    m_payload = payload;
//}
//
//unsigned int BaseMessage::getLength() {
//    return m_length;
//}
//
//string BaseMessage::getUsername() {
//    return m_username;
//}
//
//unsigned int BaseMessage::getSalt() {
//    return m_salt;
//}
//
MessageType BaseMessage::getMessageType(void* input) {
    StBaseHeader* header = (StBaseHeader*) input;
    std::string msgType(header->code);
    Direction dir;
    return convertStringToMessageType(msgType, dir);
}
//
//string BaseMessage::getStringType() {
//    return m_string_type;
//}
//
//Direction BaseMessage::getDirection() {
//    return m_dir;
//}
//
//string BaseMessage::getChatroom() {
//    return m_chatRoomName;
//}
//
//string BaseMessage::getPayloadString() {
//    return m_payload;
//}
//
//void* BaseMessage::getPayloadPtr() {
//    return static_cast<void*>(&m_payload);
//}
