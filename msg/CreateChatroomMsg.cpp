//
//  CreateChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "CreateChatroomMsg.h"

/*
CreateChatroomMsg::CreateChatroomMsg(unsigned int length, char* username,
                                     unsigned int salt, char* type,
                                     void* payload) : BaseMessage(length,
                                                username,salt,type,payload) {}
*/

CreateChatroomMsg::CreateChatroomMsg(Direction dir, MessageType type, string chatRoomName) {
    m_dir = dir;
    std::stringstream ss;
    if(dir == Direction::P2S && type == MessageType::CREATE_P2S) {
        ss<<"Create chatroom" << chatRoomName << ".";
        m_payload = ss.str();
    }
    else if (dir == Direction::S2P && type == MessageType::CREATE_S2P) {
        ss<<"Chatroom" << chatRoomName << "created";
        m_payload = ss.str();
    }
    else if (dir == Direction::S2P && type == MessageType::ERR_CREATE_S2P) {
        ss<<"Chatroom"<<chatRoomName<<"already exists; please pick another name.";
        m_payload = ss.str();
    }

}
 
CreateChatroomMsg::~CreateChatroomMsg() {}

unsigned int CreateChatroomMsg::getLength() {
    return m_length;
}

string CreateChatroomMsg::getUsername() {
    return m_username;
}

unsigned int CreateChatroomMsg::getSalt() {
    return m_salt;
}

string CreateChatroomMsg::getType() {
    return m_string_type;
}

string CreateChatroomMsg::getPayload() {
    return m_payload;
}

void CreateChatroomMsg::parseSpecificMessage(void* msg){
    BaseMessage* baseMessage  = new BaseMessage(msg);
    // getBaseMessage(msg);
    void* payload = baseMessage->getPayloadPtr();
    std::string *payload_ptr = static_cast<std::string*>(payload);
    std::string payloadStr = *payload_ptr;
    delete payload_ptr;
    std::string createMsg = "";
    if(baseMessage->getDirection() == Direction::P2S && baseMessage->getMessageType()
       == MessageType::CREATE_P2S) {
        createMsg = "Create chatroom";
    }
    else if(baseMessage->getDirection() == Direction::S2P && baseMessage -> getMessageType()
            == MessageType::CREATE_S2P) {
        createMsg = "Chatroom";
    }
    else if(baseMessage->getDirection() == Direction::S2P && baseMessage -> getMessageType()
            == MessageType::ERR_CREATE_S2P) {
        createMsg = "Chatroom";
    }
    
    std::string checkStr = payloadStr.substr(0, sizeof(createMsg));
    if (createMsg != checkStr){
        fprintf(stderr, "Incorrect payload in create chatroom message: %s", checkStr.c_str());
    }
    unsigned char* msg_char = static_cast<unsigned char*>(msg);
    memcpy(&m_chatRoomName, &msg_char[sizeof(createMsg)-1], sizeof(m_chatRoomName)-1);
}


/*
 
 Direction CreateChatroomMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "crps";
    char serverPeer[] = "crsp";
    for(int i = 0; i<(sizeof(BaseMessage::type)/BaseMessage::type[0]); ++i) {
        if(peerServer[i] == *(BaseMessage::type + i)) {
            dir = Direction::P2S;
        }
        else if(serverPeer[i] == *(BaseMessage::type + i)) {
            dir = Direction::S2P;
        }
    }
    return dir;
}

std::string CreateChatroomMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string CreateChatroomMsg::getChatRoomFromPayloadP2S() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("chatroom ");
    unsigned long first = chatRoom.find_first_of(".");
    return chatRoom.substr(last+1,first-last-1);
}

std::string CreateChatroomMsg::getChatRoomFromPayloadS2P() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("chatroom ");
    unsigned long first = chatRoom.find_first_of(" created");
    return chatRoom.substr(last+1,first-last-1);
}

*/