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

BaseMessage::BaseMessage(string username, Direction dir, string chatRoomName):
m_username(username), m_dir(dir), m_chatRoomName(chatRoomName)
{
    m_salt = rand();
}

BaseMessage::BaseMessage(void* msg)
{
    StBaseHeader* stMsg = (StBaseHeader*) msg;
    m_length = stMsg->length;
    memcpy(&m_username, stMsg->username, USERNAME_LENGTH);
    m_salt = stMsg->salt;
    m_code = stMsg->code;
    memcpy(&m_code, stMsg->code, CODE_LENGTH);
    memcpy(&m_chatRoomName, stMsg->chatRoomName, CHATROOM_NAME_LENGTH);
    m_payload = (void*)&(((char*)msg)[HEADER_LENGTH]);
    m_messageType = convertStringToMessageType(m_code, m_dir);
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
        case CHOOSE_P2S:
        case CHOOSE_S2P:
            return new ChooseUsernameMsg(input); // TODO update constructor
            break;
        case CREATE_P2S:
        case CREATE_S2P:
        case ERR_CREATE_S2P:
            return new CreateChatroomMsg(input);
        case DESTROY_P2S:
        case DESTROY_S2P:
        case ERR_DESTROY_S2P:
            return new DestroyChatroomMsg(input);
        case ENTER_P2S:
        case ENTER_S2P:
        case ERR_ENTER_S2P:
            return new EnterChatroomMsg(input);
        case LEAVE_P2S:
        case LEAVE_S2P:
            return new LeaveChatroomMsg(input);
        case LIST_P2S:
        case LIST_S2P:
            return new ListChatroomMsg(input);
        case NOTIFY_P2S:
        case NOTIFY_S2P:
        case ERR_NOTIFY_S2P:
     //       return new NotifyDroppedPeerMsg(input);
        case UPDATE_P2S:
        case UPDATE_S2P:
     //       return new UpdateRecipientsMsg(input);
        // TODO add other cases
        default:
            fprintf(stderr, "Error creating new message!\n");
            return nullptr;
            break;
    }
}

//message_t* BaseMessage::getMessageStruct() {
//    message_t* retStruct = (message_t*)malloc(sizeof(HEADER_LENGTH) + sizeof(m_payload));
//    StBaseMessage.length = length;
//    
//    
//    
//    memcpy(StBaseMessage.username, m_username.c_str(), sizeof(StBaseMessage.username));
//    //Buffer username with null bytes
//    if(strlen(StBaseMessage.username) < 16) {
//        memset(StBaseMessage.username + strlen(m_username.c_str()), '0',
//               16 - strlen(m_username.c_str()));
//    }
//    
//    
//    StBaseMessage.salt = m_salt;
//    memcpy(StBaseMessage.type, m_string_type.c_str(), sizeof(StBaseMessage.type));
//    
//    memcpy(StBaseMessage.chatroom, m_chatRoomName.c_str(), sizeof(StBaseMessage.chatroom));
//    //Buffer chatroom with null bytes
//    if(strlen(StBaseMessage.chatroom) < 16) {
//        memset(StBaseMessage.chatroom + strlen(m_chatRoomName.c_str()), '0',
//               16 - strlen(m_chatRoomName.c_str()));
//    }
//    
//    
//    memcpy(StBaseMessage.payload, m_payload.c_str(), sizeof(StBaseMessage.payload));
//    
//    memcpy(retStruct, &StBaseMessage, sizeof(&retStruct));
//    return retStruct;
//    
//}

static string log(BaseMessage* msg) {
    std::stringstream ss;
    ss << "The length is: " << msg->getLength() << std::endl;
    ss << "The username is: " << msg->getUsername() << std::endl;
   // ss << "The salt is: " << msg->getSalt() << std::endl;
 //   ss << "The message type is: " << msg->getMessageType() << std::endl;
    ss << "The string type is: " << msg->getMessageCode() << std::endl;
    ss << "The direction is: " << static_cast<int>(msg->getDirection()) << std::endl;
    ss << "The chatroom name is: " << msg->getChatRoomName() << std::endl;
    ss << "The payload is: " << msg->getPayloadPtr() << std::endl;
    return ss.str();
}

StBaseHeader* BaseMessage::getHeaderStruct(){
    StBaseHeader* retStruct = (StBaseHeader*) malloc(sizeof(HEADER_LENGTH));
    memset((void*)retStruct, 0, sizeof(retStruct)); // ensures extra 0s for char[]s
    // TODO set length in children classes
    const char* tempUsername = m_username.c_str();
    memcpy(&(retStruct->username), &tempUsername, m_username.size());
    retStruct->salt = m_salt;
    // TODO set code in children classes
    const char* tempChatRoomName = m_chatRoomName.c_str();
    memcpy(&(retStruct->chatRoomName), &tempChatRoomName, m_chatRoomName.size());
    return retStruct;
}

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
unsigned int BaseMessage::getLength() {
    return m_length;
}
//
string BaseMessage::getUsername() {
    return m_username;
}
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


string BaseMessage::getMessageCode() {
    return m_code;
}

Direction BaseMessage::getDirection() {
    return m_dir;
}

string BaseMessage::getChatRoomName() {
    return m_chatRoomName;
}

void* BaseMessage::getPayloadPtr() {
    return m_payload;
}

MessageType BaseMessage::getMessageType(){
	return m_messageType;
}
