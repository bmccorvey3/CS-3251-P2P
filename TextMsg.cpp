//
//  TextMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "TextMsg.h"

TextMsg::TextMsg(unsigned int length, char* username, unsigned int salt,
                 char* type, void* payload) : BaseMessage(length,username,salt,
                                                          type,payload) {}

TextMsg::~TextMsg() {}

unsigned int TextMsg::getLength() {
    return BaseMessage::l;
}

char* TextMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int TextMsg::getSalt() {
    return BaseMessage::salt;
}

char* TextMsg::getType() {
    return BaseMessage::type;
}

void* TextMsg::getPayload() {
    return BaseMessage::payload;
}

Direction TextMsg::getDirection() {
    return Direction::TEXT;
}

std::string TextMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string TextMsg::getPayloadText() {
    std::string payload = getStringFromPayload(BaseMessage::payload);
    unsigned long first = payload.find_last_of(": ");
    return payload.substr(first+1);
}


