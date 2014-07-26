//
//  CreateChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "CreateChatroomMsg.h"

const string CreateChatroomMsg::m_prefixP2S = "Create chatroom";
const string CreateChatroomMsg::m_prefixS2P = "Chatroom";
const string CreateChatroomMsg::m_prefixS2P_ERR = "Chatroom";
const string CreateChatroomMsg::m_postfixP2S = ".";
const string CreateChatroomMsg::m_postFixS2P = "created.";
const string CreateChatroomMsg::m_postfixS2P_ERR = "already exists; please pick another name" ;

CreateChatroomMsg::CreateChatroomMsg(string username, Direction dir, string chatRoomName, string payload)
: BaseMessage(username, dir, chatRoomName)
{
    m_payloadString = payload;
    m_chatRoomNamePayload = payload;
    if(dir == Direction::P2S) {
        m_messageType = MessageType::CREATE_P2S;
        m_length = HEADER_LENGTH + p2sTotalPayloadSize;
        m_code = "crps";
    }
    else if (dir == Direction::S2P) {
        m_messageType = MessageType::CREATE_S2P;
        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
        m_code = "crsp";
    }
    else if (dir == Direction::ERROR) {
        m_messageType = MessageType::ERR_CREATE_S2P;
        m_length =  HEADER_LENGTH + s2pErrTotalPayloadSize;
        m_code = "cfsp";
    }
    else {
        fprintf(stderr, "Incorrect direction in CreateChatRoom!\n");
    }
}

CreateChatroomMsg::~CreateChatroomMsg() {}

CreateChatroomMsg::CreateChatroomMsg(void* input) : BaseMessage(input) {
    char* chatRoomPayload = (char*) malloc(m_length);
    memcpy(chatRoomPayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempChatRoomPayload(chatRoomPayload);
    if(tempChatRoomPayload.find(m_prefixP2S) > 0){
        m_chatRoomNamePayload = string(chatRoomPayload);
        // cut down on string to what we actuall want
        m_chatRoomNamePayload = m_chatRoomNamePayload.substr(m_prefixP2S.size(), m_chatRoomName.size());
        // TODO deallocate m_textPayload in destructor
    }
    else if (tempChatRoomPayload.find(m_prefixS2P) > 0) {
        m_chatRoomNamePayload = string(chatRoomPayload);
        m_chatRoomNamePayload = m_chatRoomNamePayload.substr(m_prefixS2P.size(), m_chatRoomName.size());
    }
    else if (tempChatRoomPayload.find(m_postfixS2P_ERR) > 0){
        m_chatRoomNamePayload = string(chatRoomPayload);
        m_chatRoomNamePayload = m_chatRoomNamePayload.substr(m_prefixS2P.size(), m_chatRoomName.size());
    }
    else {
        fprintf(stderr, "Couldn't find in create chatroom payload\n");
        // TODO add log information
    }
    // free choosePayload; // TODO do I need this?
}

void* CreateChatroomMsg::getMessageStruct() {
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
    else if (m_dir == Direction::ERROR) {
        fullMessage = (FullMessageS2PErr*) malloc(sizeof(FullMessageS2PErr));
    }
    memcpy(fullMessage,&header,sizeof(StBaseHeader));
    string tempPayloadStr;
    if(m_dir == Direction::P2S) {
        tempPayloadStr = m_prefixP2S + m_chatRoomNamePayload + m_postfixP2S;
    }
    else if (m_dir == Direction::S2P) {
        tempPayloadStr = m_prefixS2P + m_chatRoomNamePayload + m_postFixS2P;
    }
    else if (m_dir == Direction::ERROR) {
        tempPayloadStr = m_prefixS2P_ERR + m_chatRoomNamePayload + m_postfixS2P_ERR;
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

string CreateChatroomMsg::getChatRoomNamePayload() {
    return m_chatRoomNamePayload;
}

string CreateChatroomMsg::getPayloadString() {
    return m_payloadString;
}



