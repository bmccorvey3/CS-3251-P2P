//
//  EnterChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "EnterChatroomMsg.h"

const string EnterChatroomMsg::m_prefixP2S = " wants to enter chatroom." ;
const string EnterChatroomMsg::m_prefixS2P = "Entered chatroom ";
const string EnterChatroomMsg::m_S2P_ERR = "Choose a username.";
const string EnterChatroomMsg::m_postfixS2P = ".";

EnterChatroomMsg::EnterChatroomMsg(string username, Direction dir, string chatRoomName, string payload)
: BaseMessage(username, dir, chatRoomName)
{
    m_payloadString = payload;
    m_enterPayload = payload;
    if(dir == Direction::P2S) {
        m_messageType = MessageType::ENTER_P2S;
        m_length = HEADER_LENGTH + p2sTotalPayloadSize;
        m_code = "enps";
    }
    else if (dir == Direction::S2P) {
        m_messageType = MessageType::ENTER_S2P;
        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
        m_code = "ensp";
    }
    else if (dir == Direction::ERROR) {
        m_messageType = MessageType::ERR_ENTER_S2P;
        m_length = HEADER_LENGTH + s2pErrTotalPayloadSize;
        m_code = "efsp";
    }
    else {
        fprintf(stderr, "Incorrect direction in EnterChatroomMsg!\n");
    }
}

EnterChatroomMsg::~EnterChatroomMsg() {}

EnterChatroomMsg::EnterChatroomMsg(void* input) : BaseMessage(input) {
    char* chatRoomPayload = (char*) malloc(m_length);
    memcpy(chatRoomPayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempChatRoomPayload(chatRoomPayload);
    if(tempChatRoomPayload.find(m_prefixP2S) > 0){
        m_enterPayload = string(chatRoomPayload);
        // cut down on string to what we actuall want
        m_enterPayload = m_enterPayload.substr(0, m_username.size());
        // TODO deallocate m_textPayload in destructor
    }
    else if (tempChatRoomPayload.find(m_prefixS2P) > 0) {
        m_enterPayload = string(chatRoomPayload);
        m_enterPayload = m_enterPayload.substr(m_prefixS2P.size(), m_chatRoomName.size());
    }
    else if (tempChatRoomPayload.find(m_S2P_ERR) > 0){
        m_enterPayload = string(chatRoomPayload);
    }
    else {
        fprintf(stderr, "Couldn't find in enter chatroom payload\n");
        // TODO add log information
    }
    // free choosePayload; // TODO do I need this?
}

void* EnterChatroomMsg::getMessageStruct() {
    StBaseHeader* header = BaseMessage::getHeaderStruct();
    header->length = m_length;
    memcpy(&(header->code), &m_code, CODE_LENGTH);
    void* fullMessage;
    if(m_dir == Direction::P2S) {
        fullMessage = (FullMessageP2S*) malloc(sizeof(FullMessageP2S));
    }
    else if (m_dir == Direction::S2P){
        fullMessage = (FullMessageS2P*) malloc(sizeof(FullMessageS2P));
    }
    else if (m_dir == Direction::ERROR) {
        fullMessage = (FullMessageS2PErr*) malloc(sizeof(FullMessageS2PErr));
    }
    memcpy(fullMessage,&header,sizeof(StBaseHeader));
    string tempPayloadStr;
    if(m_dir == Direction::P2S) {
        tempPayloadStr = m_prefixP2S + m_enterPayload;
    }
    else if (m_dir == Direction::S2P) {
        tempPayloadStr = m_prefixS2P + m_enterPayload + m_postfixS2P;
    }
    else if (m_dir == Direction::ERROR) {
        tempPayloadStr = m_S2P_ERR;
    }
    const char* tempPayloadStr_cstr = tempPayloadStr.c_str();
    if(m_dir == Direction::P2S) {
        memcpy(&((FullMessageP2S*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, p2sTotalPayloadSize);
    }
    else if(m_dir == Direction::S2P) {
        memcpy(&((FullMessageS2P*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, s2pTotalPayloadSize);
    }
    else if(m_dir == Direction::ERROR) {
        memcpy(&((FullMessageS2PErr*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, s2pErrTotalPayloadSize);
    }
    
    free(header);
    return fullMessage;
}

string EnterChatroomMsg::getEnterPayload() {
    return m_enterPayload;
}

string EnterChatroomMsg::getPayloadString() {
    return m_payloadString;
}

