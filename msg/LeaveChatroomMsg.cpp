//
//  LeaveChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "LeaveChatroomMsg.h"

LeaveChatroomMsg::LeaveChatroomMsg(unsigned int length, char* username,
                                   unsigned int salt, char* type, void* payload)
                            : BaseMessage(length,username,salt,type,payload) {}

LeaveChatroomMsg::~LeaveChatroomMsg() {}

unsigned int LeaveChatroomMsg::getLength() {
    return BaseMessage::l;
}

char* LeaveChatroomMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int LeaveChatroomMsg::getSalt() {
    return BaseMessage::salt;
}

char* LeaveChatroomMsg::getType() {
    return BaseMessage::type;
}

void* LeaveChatroomMsg::getPayload() {
    return BaseMessage::payload;
}

Direction LeaveChatroomMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "lvps";
    char serverPeer[] = "lvsp";
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

std::string LeaveChatroomMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string LeaveChatroomMsg::getUserNameFromPayload() {
    std::string username = getStringFromPayload(BaseMessage::payload);
    unsigned long first = username.find_first_of(" ");
    return username.substr(0,first);
}

std::string LeaveChatroomMsg::getChatRoomFromPayload() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("left ");
    unsigned long first = chatRoom.find_first_of(" chatroom");
    return chatRoom.substr(last+1,first-last-1);
}
