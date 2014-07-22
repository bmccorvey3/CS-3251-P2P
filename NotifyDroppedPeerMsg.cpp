//
//  NotifyDroppedPeerMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "NotifyDroppedPeerMsg.h"

NotifyDroppedPeerMsg::NotifyDroppedPeerMsg(unsigned int length, char* username,
                                           unsigned int salt,char* type,
                                           void* payload) :
                                            BaseMessage(length,username,salt,
                                                        type,payload) {}

NotifyDroppedPeerMsg::~NotifyDroppedPeerMsg() {}

unsigned int NotifyDroppedPeerMsg::getLength() {
    return BaseMessage::l;
}

char* NotifyDroppedPeerMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int NotifyDroppedPeerMsg::getSalt() {
    return BaseMessage::salt;
}

char* NotifyDroppedPeerMsg::getType() {
    return BaseMessage::type;
}

void* NotifyDroppedPeerMsg::getPayload() {
    return BaseMessage::payload;
}

Direction NotifyDroppedPeerMsg::getDirection() {
    Direction dir = Direction::NONE;
    char peerServer[] = "pdsp";
    char serverPeer[] = "pdsp";
    char serverPeer2[] = "pksp";
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

std::string NotifyDroppedPeerMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string NotifyDroppedPeerMsg::getDroppedIpP2S() {
    std::string dropped = getStringFromPayload(BaseMessage::payload);
    unsigned long last = dropped.find_last_of("Peer ");
    unsigned long first = dropped.find_first_of(":");
    return dropped.substr(last+1,first-last-1);
}

std::string NotifyDroppedPeerMsg::getDroppedIpS2P() {
    std::string dropped = getStringFromPayload(BaseMessage::payload);
    unsigned long last = dropped.find_last_of("Removing peer ");
    unsigned long first = dropped.find_first_of(".");
    return dropped.substr(last+1,first-last-1);
}

std::string NotifyDroppedPeerMsg::getDroppedPortP2S() {
    std::string port = getStringFromPayload(BaseMessage::payload);
    unsigned long last = port.find_last_of(":");
    unsigned long first = port.find_first_of(" dropped");
    return port.substr(last+1,first-last-1);
}