//
//  UpdateRecipientsMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "UpdateRecipientsMsg.h"

UpdateRecipientsMsg::UpdateRecipientsMsg(unsigned int length, char* username,
                                         unsigned int salt, char* type,
                                         void* payload) : BaseMessage(length,
                                                        username,salt,type,
                                                        payload) {}

UpdateRecipientsMsg::~UpdateRecipientsMsg() {}

unsigned int UpdateRecipientsMsg::getLength() {
    return BaseMessage::l;
}

char* UpdateRecipientsMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int UpdateRecipientsMsg::getSalt() {
    return BaseMessage::salt;
}

char* UpdateRecipientsMsg::getType() {
    return BaseMessage::type;
}

void* UpdateRecipientsMsg::getPayload() {
    return BaseMessage::payload;
}

Direction UpdateRecipientsMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "upps";
    char serverPeer[] = "upsp";
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

std::string UpdateRecipientsMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string UpdateRecipientsMsg::getPrimaryRecipients() {
    std::string primary = getStringFromPayload(BaseMessage::payload);
    unsigned long last = primary.find_last_of("New peers: ");
    unsigned long first = primary.find_first_of(",");
    return primary.substr(last+1,first-last-1);
}

std::string UpdateRecipientsMsg::getSecondaryRecipients() {
    std::string secondary = getStringFromPayload(BaseMessage::payload);
    unsigned long last = secondary.find_last_of(",");
    unsigned long first = secondary.find_first_of(".");
    return secondary.substr(last+1,first-last-1);
}
