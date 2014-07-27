//
//  LeaveChatroomMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "LeaveChatroomMsg.h"

const string LeaveChatroomMsg::m_prefixS2P = " left ";
const string LeaveChatroomMsg::m_postfixP2S = " wants to leave chatroom." ;
const string LeaveChatroomMsg::m_postfixS2P = " chatroom.";


LeaveChatroomMsg::LeaveChatroomMsg(string username, Direction dir, string chatRoomName)
: BaseMessage(username, dir, chatRoomName)
{
    if(dir == Direction::P2S) {
        m_messageType = MessageType::LEAVE_P2S;
        m_length = HEADER_LENGTH + p2sTotalPayloadSize;
        m_code = "lvps";
    }
    else if (dir == Direction::S2P) {
        m_messageType = MessageType::LEAVE_S2P;
        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
        m_code = "lvsp";
    }
    else {
        fprintf(stderr, "Incorrect direction in LeaveChatroom!\n");
    }
}

LeaveChatroomMsg::~LeaveChatroomMsg() {}

LeaveChatroomMsg::LeaveChatroomMsg(void* input) : BaseMessage(input) {
	// Don't need this since it's a sanity check

//    char* chatRoomPayload = (char*) malloc(m_length);
//    memcpy(chatRoomPayload, &((char*)input)[HEADER_LENGTH], m_length);
//    std::string tempchatRoomPayload(chatRoomPayload);
//    if(tempchatRoomPayload.find(m_postfixP2S) > 0){
//        m_payloadString = string(chatRoomPayload);
//        // cut down on string to what we actuall want
//        m_payloadString = m_payloadString.substr(0, m_username.size());
//        // TODO deallocate m_textPayload in destructor
//    }
//    else if (tempchatRoomPayload.find(m_prefixS2P) > 0) {
//        m_payloadString = string(chatRoomPayload);
//        m_payloadString = m_payloadString.substr(0, m_username.size());
//        m_payloadString = string(chatRoomPayload);
//        m_payloadString = m_payloadString.substr
//                                (m_username.size()+sizeof(m_prefixS2P),m_chatRoomName.size());
//    }
//    else {
//        fprintf(stderr, "Couldn't find in leave chatroom payload\n");
//        // TODO add log information
//    }
//    // free choosePayload; // TODO do I need this?

}

void* LeaveChatroomMsg::getMessageStruct() {
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
    memcpy(&fullMessage,&header,sizeof(StBaseHeader));
    string tempPayloadStr;
    if(m_dir == Direction::P2S) {
        tempPayloadStr = m_username + m_postfixP2S;
    }
    else if (m_dir == Direction::S2P) {
        tempPayloadStr = m_username + m_prefixS2P + m_chatRoomName + m_postfixS2P;
    }
    const char* tempPayloadStr_cstr = tempPayloadStr.c_str();
    if(m_dir == Direction::P2S) {
        memcpy(&((FullMessageP2S*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, p2sTotalPayloadSize);
    }
    else if(m_dir == Direction::S2P) {
        memcpy(&((FullMessageS2P*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, s2pTotalPayloadSize);
    }
   // free(header);
    return fullMessage;
}

string LeaveChatroomMsg::getPayloadString() {
    return m_payloadString;
}
