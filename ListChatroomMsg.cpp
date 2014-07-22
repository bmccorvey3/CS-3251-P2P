//
//  ListChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "ListChatroomMsg.h"

ListChatroomMsg::ListChatroomMsg(unsigned int length, char* username,
                                 unsigned int salt,char* type, void* payload)
                            : BaseMessage(length,username,salt,type,payload) {}

ListChatroomMsg::~ListChatroomMsg() {}

unsigned int ListChatroomMsg::getLength() {
    return BaseMessage::l;
}

char* ListChatroomMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int ListChatroomMsg::getSalt() {
    return BaseMessage::salt;
}

char* ListChatroomMsg::getType() {
    return BaseMessage::type;
}

void* ListChatroomMsg::getPayload() {
    return BaseMessage::payload;
}

Direction ListChatroomMsg::getDirection() {
    Direction dir = Direction::NONE;
    char peerServer[] = "lsps";
    char serverPeer[] = "lssp";
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

std::string ListChatroomMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string ListChatroomMsg::getListofChatrooms() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("Chatrooms: ");
    unsigned long first = chatRoom.find_first_of(".");
    return chatRoom.substr(last+1,first-last-1);
}