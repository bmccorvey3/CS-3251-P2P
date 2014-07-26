//
//  TextMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "TextMsg.h"
#define POS_TXT_PREFIX 0 // position of "txt:"

TextMsg::TextMsg(unsigned int length, char* username, unsigned int salt,
                 char* type, void* payload) : BaseMessage(length,username,salt,
                                                          type,payload) {}
const std::string TextMsg::m_prefixStr = "txt:";
TextMsg::~TextMsg() {}

TextMsg::TextMsg(void* input) : BaseMessage(input){
    char* textPayload = (char*) malloc(m_length);
    memcpy(textPayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempTextPayload(textPayload);
    if(tempTextPayload.find(m_prefixStr) == POS_TXT_PREFIX){
        m_textPayload = std::string(textPayload);
        // cut down on string to what we actuall want
        m_textPayload = m_textPayload.substr(POS_TXT_PREFIX+m_prefixStr.size());
        // TODO deallocate m_textPayload in destructor
    } else {
        fprintf(stderr, "Couldn't find 'txt:' in text payload\n");
        // TODO add log information
    }
    // free textPayload; // TODO do I need this?
}

//unsigned int TextMsg::getLength() {
//    return BaseMessage::l;
//}
//
//char* TextMsg::getUsername() {
//    return BaseMessage::user;
//}
//
//unsigned int TextMsg::getSalt() {
//    return BaseMessage::salt;
//}
//
//char* TextMsg::getType() {
//    return BaseMessage::type;
//}

void* TextMsg::getPayload() {
    return BaseMessage::payload;
}

Direction TextMsg::getDirection() {
    return Direction::P2P;
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


