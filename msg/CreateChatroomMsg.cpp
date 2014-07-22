//
//  CreateChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "CreateChatroomMsg.h"

CreateChatroomMsg::CreateChatroomMsg(unsigned int length, char* username,
                                     unsigned int salt, char* type,
                                     void* payload) : BaseMessage(length,
                                                username,salt,type,payload) {}

CreateChatroomMsg::~CreateChatroomMsg() {}

unsigned int CreateChatroomMsg::getLength() {
    return BaseMessage::l;
}

char* CreateChatroomMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int CreateChatroomMsg::getSalt() {
    return BaseMessage::salt;
}

char* CreateChatroomMsg::getType() {
    return BaseMessage::type;
}

void* CreateChatroomMsg::getPayload() {
    return BaseMessage::payload;
}

Direction CreateChatroomMsg::getDirection() {
    Direction dir = Direction::NONE;
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