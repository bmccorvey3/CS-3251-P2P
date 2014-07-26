//
//  DestroyChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "DestroyChatroomMsg.h"

/*DestroyChatroomMsg::DestroyChatroomMsg(unsigned int length, char* username,
                                       unsigned int salt, char* type,
                                       void* payload) : BaseMessage(length,
                                                username,salt,type,payload) {}*/

DestroyChatroomMsg::DestroyChatroomMsg(Direction dir, MessageType type, string chatRoomName) {
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

DestroyChatroomMsg::~DestroyChatroomMsg() {}

unsigned int DestroyChatroomMsg::getLength() {
    return m_length;
}

string DestroyChatroomMsg::getUsername() {
    return m_username;
}

unsigned int DestroyChatroomMsg::getSalt() {
    return m_salt;
}

string DestroyChatroomMsg::getType() {
    return m_string_type;
}

string DestroyChatroomMsg::getPayload() {
    return m_payload;
}

/*

Direction DestroyChatroomMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "dsps";
    char serverPeer[] = "dssp";
    char serverPeer2[] = "dnsp";
    for(int i = 0; i<(sizeof(BaseMessage::type)/BaseMessage::type[0]); ++i) {
        if(peerServer[i] == *(BaseMessage::type + i)) {
            dir = Direction::P2S;
        }
        else if(serverPeer[i] == *(BaseMessage::type + i) ||
                serverPeer2[i] == *(BaseMessage::type + i)) {
            dir = Direction::S2P;
        }
    }
    return dir;
}

std::string DestroyChatroomMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string DestroyChatroomMsg::getDestroyedChatRoomP2S() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("chatroom ");
    unsigned long first = chatRoom.find_first_of(".");
    return chatRoom.substr(last+1,first-last-1);
}

std::string DestroyChatroomMsg::getDestroyedChatRoomS2P() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("Chatroom ");
    unsigned long first = chatRoom.find_first_of(" destroyed");
    return chatRoom.substr(last+1,first-last-1);
}

*/