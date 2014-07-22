//
//  ChooseUsernameMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "ChooseUsernameMsg.h"

ChooseUsernameMsg::ChooseUsernameMsg(unsigned int length, char* username,
                                     unsigned int salt, char* type,
                                     void* payload) : BaseMessage(length,
                                                username,salt,type,payload) {}

ChooseUsernameMsg::~ChooseUsernameMsg() {}

unsigned int ChooseUsernameMsg::getLength() {
    return BaseMessage::l;
}

char* ChooseUsernameMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int ChooseUsernameMsg::getSalt() {
    return BaseMessage::salt;
}

char* ChooseUsernameMsg::getType() {
    return BaseMessage::type;
}

void* ChooseUsernameMsg::getPayload() {
    return BaseMessage::payload;
}

Direction ChooseUsernameMsg::getDirection() {
    Direction dir = Direction::NONE;
    char peerServer[] = "cups";
    char serverPeer[] = "cusp";
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

std::string ChooseUsernameMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string ChooseUsernameMsg::getUsernameP2S() {
    std::string username = getStringFromPayload(BaseMessage::payload);
    unsigned long last = username.find_last_of("My username: ");
    return username.substr(last+1);
}

std::string ChooseUsernameMsg::getUsernameS2P() {
    std::string username = getStringFromPayload(BaseMessage::payload);
    unsigned long last = username.find_last_of("Hello, ");
    return username.substr(last+1);
}