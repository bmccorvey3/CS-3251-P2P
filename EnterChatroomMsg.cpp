//
//  EnterChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "EnterChatroomMsg.h"

EnterChatroomMsg::EnterChatroomMsg(unsigned int length, char* username,
                                   unsigned int salt,char* type, void* payload)
                            : BaseMessage(length,username,salt,type,payload) {}

EnterChatroomMsg::~EnterChatroomMsg() {}

unsigned int EnterChatroomMsg::getLength() {
    return BaseMessage::l;
}

char* EnterChatroomMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int EnterChatroomMsg::getSalt() {
    return BaseMessage::salt;
}

char* EnterChatroomMsg::getType() {
    return BaseMessage::type;
}

void* EnterChatroomMsg::getPayload() {
    return BaseMessage::payload;
}

Direction EnterChatroomMsg::getDirection() {
    Direction dir = Direction::NONE;
    char peerServer[] = "enps";
    char serverPeer[] = "ensp";
    char serverPeer2[] = "efsp";
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

std::string EnterChatroomMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string EnterChatroomMsg::getChatRoomFromPayload() {
    std::string chatRoom = getStringFromPayload(BaseMessage::payload);
    unsigned long last = chatRoom.find_last_of("chatroom ");
    unsigned long first = chatRoom.find_first_of(".");
    return chatRoom.substr(last+1,first-last-1);
}
