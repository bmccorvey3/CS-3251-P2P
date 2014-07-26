//
//  ChooseUsernameMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "ChooseUsernameMsg.h"
#define POS_CHOOSE_PREFIX 0
const string ChooseUsernameMsg::m_prefixP2S = "My username:";
const string ChooseUsernameMsg::m_prefixS2P = "Hello, ";

ChooseUsernameMsg::ChooseUsernameMsg(string username, Direction dir, string payload)
                    : BaseMessage(username, dir, NULL)
{
    m_payloadString = payload;
    m_choosePayload = payload;
    if(dir == Direction::P2S) {
        m_messageType = MessageType::CHOOSE_P2S;
        m_length = HEADER_LENGTH + p2sTotalPayloadSize;
        m_code = "cups";
    }
    else if (dir == Direction::S2P) {
        m_messageType = MessageType::CHOOSE_S2P;
        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
        m_code = "cusp";
    }
    else {
        fprintf(stderr, "Incorrect direction in ChooseUsername!\n");
    }
}

ChooseUsernameMsg::~ChooseUsernameMsg() {}

ChooseUsernameMsg::ChooseUsernameMsg(void* input) : BaseMessage(input) {
    char* choosePayload = (char*) malloc(m_length);
    memcpy(choosePayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempchoosePayload(choosePayload);
    if(tempchoosePayload.find(m_prefixP2S) == POS_CHOOSE_PREFIX){
        m_choosePayload = string(choosePayload);
        // cut down on string to what we actuall want
        m_choosePayload = m_choosePayload.substr(POS_CHOOSE_PREFIX+m_prefixP2S.size());
        // TODO deallocate m_textPayload in destructor
    }
    else if (tempchoosePayload.find(m_prefixS2P) == POS_CHOOSE_PREFIX) {
        m_choosePayload = string(choosePayload);
        m_choosePayload = m_choosePayload.substr(POS_CHOOSE_PREFIX+m_prefixS2P.size());
    }
    else {
        fprintf(stderr, "Couldn't find in choose username payload\n");
        // TODO add log information
    }
    // free choosePayload; // TODO do I need this?
}

void* ChooseUsernameMsg::getMessageStruct() {
    StBaseHeader* header = BaseMessage::getHeaderStruct();
    header->length = m_length;
    memcpy(&(header->code), &m_code, CODE_LENGTH);
    void* fullMessage;
    if(m_dir == Direction::P2S) {
        fullMessage = (FullMessageP2S*)malloc(sizeof(FullMessageP2S));
    }
    else if (m_dir == Direction::S2P){ // S2P
        fullMessage = (FullMessageS2P*)malloc(sizeof(FullMessageS2P));
    }
    memcpy(fullMessage,&header,sizeof(StBaseHeader));
    string tempPayloadStr;
    if(m_dir == Direction::P2S) {
        tempPayloadStr = m_prefixP2S + m_choosePayload;
    }
    else if (m_dir == Direction::S2P) {
        tempPayloadStr = m_prefixS2P + m_choosePayload;
    }
    const char* tempPayloadStr_cstr = tempPayloadStr.c_str();
    if(m_dir == Direction::P2S) {
        memcpy(&((FullMessageP2S*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, p2sTotalPayloadSize);
    }
    else if(m_dir == Direction::S2P) {
        memcpy(&((FullMessageS2P*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, s2pTotalPayloadSize);
    }
    free(header);
    return fullMessage;
}

string ChooseUsernameMsg::getPayloadString() {
    return m_payloadString;
}

string ChooseUsernameMsg::getChooseString() {
    return m_choosePayload;
}



