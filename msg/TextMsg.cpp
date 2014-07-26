//
//  TextMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "TextMsg.h"
#define POS_TXT_PREFIX 0 // position of "txt:"
const std::string TextMsg::m_prefixStr = "txt:";

TextMsg::TextMsg(std::string username, std::string chatRoomName, std::string payload) : BaseMessage(username,Direction::P2P, chatRoomName)
{
    m_textPayload = payload;
    m_messageType = MessageType::TEXT;
    m_length = HEADER_LENGTH + sizeof(m_textPayload) + sizeof(m_prefixStr);
    m_code = "text";
}

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

void* TextMsg::getMessageStruct()
{
    StBaseHeader* header = BaseMessage::getHeaderStruct();
    header->length = m_length;
    memcpy(&(header->code), &m_code, CODE_LENGTH);
    const size_t totalPayloadSize =sizeof(m_textPayload)+ sizeof(m_prefixStr);
    typedef struct FULL_MESSAGE {
        StBaseHeader stBaseHeader;
        char payload[totalPayloadSize];
    } FullMessage;
    FullMessage* fullMessage = (FullMessage*)malloc(sizeof(FullMessage));
    memcpy(fullMessage,&header,sizeof(StBaseHeader));
    std::string tempPayloadStr = m_prefixStr + m_textPayload;
    const char* tempPayloadStr_cstr = tempPayloadStr.c_str();
    memcpy(&fullMessage[sizeof(StBaseHeader)], &tempPayloadStr_cstr, totalPayloadSize);
    free(header);
    return fullMessage;
}
std::string TextMsg::getTextPayload() {
    return m_textPayload;
}

std::string TextMsg::getPrefixStr() {
    return m_prefixStr;
}

//std::string TextMsg::getPayloadText() {
//    std::string payload = getStringFromPayload(BaseMessage::payload);
//    unsigned long first = payload.find_last_of(": ");
//    return payload.substr(first+1);
//}