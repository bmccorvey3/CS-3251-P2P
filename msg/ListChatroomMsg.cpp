//
//  ListChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "ListChatroomMsg.h"

const string ListChatroomMsg::m_P2S = "List chatrooms.";
const string ListChatroomMsg::m_prefixS2P = "Chatrooms:";
const string ListChatroomMsg::m_postFixS2P = ".";


ListChatroomMsg::ListChatroomMsg(Direction dir, string chatRoomlist)
: BaseMessage(NULL, dir, chatRoomlist)
{
    m_payloadString = chatRoomlist;
    m_listPayload = chatRoomlist;
    if(dir == Direction::P2S) {
        m_messageType = MessageType::LIST_P2S;
        m_length = HEADER_LENGTH + p2sTotalPayloadSize;
        m_code = "lsps";
    }
    else if (dir == Direction::S2P) {
        m_messageType = MessageType::LIST_S2P;
        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
        m_code = "lssp";
    }
    else {
        fprintf(stderr, "Incorrect direction in ListChatroom!\n");
    }
}

ListChatroomMsg::~ListChatroomMsg() {}

ListChatroomMsg::ListChatroomMsg(void* input) : BaseMessage(input) {
    char* chatRoomPayload = (char*) malloc(m_length);
    memcpy(chatRoomPayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempchatRoomPayload(chatRoomPayload);
    if(tempchatRoomPayload.find(m_prefixS2P) > 0){
        m_listPayload = string(chatRoomPayload);
        // cut down on string to what we actuall want
        m_listPayload = m_listPayload.substr(sizeof(m_prefixS2P),m_listPayload.size());
        // TODO deallocate m_textPayload in destructor
    }
    else {
        fprintf(stderr, "Couldn't find in list username payload\n");
        // TODO add log information
    }
    // free choosePayload; // TODO do I need this?
}

void* ListChatroomMsg::getMessageStruct() {
    StBaseHeader* header = BaseMessage::getHeaderStruct();
    header->length = m_length;
    memcpy(&(header->code), &m_code, CODE_LENGTH);
    void* fullMessage;
    if(m_dir == Direction::P2S) {
        fullMessage = (FullMessageP2S*)malloc(sizeof(FullMessageP2S));
    }
    else if (m_dir == Direction::S2P){
        fullMessage = (FullMessageS2P*)malloc(sizeof(FullMessageS2P));
    }
    memcpy(fullMessage,&header,sizeof(StBaseHeader));
    string tempPayloadStr;
    if(m_dir == Direction::P2S) {
        tempPayloadStr = m_P2S;
    }
    else if (m_dir == Direction::S2P) {
        tempPayloadStr = m_prefixS2P + m_listPayload + m_postFixS2P;
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

string ListChatroomMsg::getListPayload() {
    return m_listPayload;
}


string ListChatroomMsg::getPayloadString() {
    return m_payloadString;
}
