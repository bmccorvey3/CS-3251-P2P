//
//  TextMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "TextMsg.h"
#define POS_TXT_PREFIX 0 // position of "txt:"
const string TextMsg::m_prefixStr = "txt:\0";

TextMsg::TextMsg(string username, string chatRoomName, string payload) : BaseMessage(username,Direction::P2P, chatRoomName)
{
    m_payloadString = payload;
    m_messageType = MessageType::TEXT;
    m_length = (unsigned int) (HEADER_LENGTH + m_payloadString.length() + m_prefixStr.length());
    m_code = "text";
}

TextMsg::~TextMsg() {}

TextMsg::TextMsg(void* input) : BaseMessage(input){
//    char* textPayload = (char*) malloc(m_length);
//    memcpy(textPayload, &((char*)input)[HEADER_LENGTH], m_length);
//    std::string tempTextPayload(textPayload);
//    if(tempTextPayload.find(m_presfixStr) == POS_TXT_PREFIX){
//        m_payloadString = std::string(textPayload);
//        // cut down on string to what we actuall want
//        m_payloadString = m_payloadString.substr(POS_TXT_PREFIX+m_prefixStr.size());
//        // TODO deallocate m_payloadString in destructor
//    } else {
//        fprintf(stderr, "Couldn't find 'txt:' in text payload\n");
//        // TODO add log information
//    }
//    // free textPayload; // TODO do I need this?
}

void* TextMsg::getMessageStruct()
{
    StBaseHeader* header = BaseMessage::getHeaderStruct();
    header->length = m_length;
    memcpy(&(header->code), &m_code, CODE_LENGTH);

//    std::cout << "Length of payload: " << m_payloadString.length() << std::endl
//    << "Length of prefix str for TextMsg: " << m_prefixStr.le << std::endl;
    const size_t totalPayloadSize = m_payloadString.length()+ m_prefixStr.length();
//    typedef struct __attribute__((packed)) FULL_MESSAGE {
//        StBaseHeader stBaseHeader;
//        char payload[totalPayloadSize];
//    } FullMessage;
    std::cout << "totalPayloadSize: " << totalPayloadSize << std::endl;
    void* fullMessage = malloc(totalPayloadSize); //ERROR
    memcpy(&fullMessage,&header,sizeof(StBaseHeader));
    std::string tempPayloadStr = m_prefixStr + m_payloadString;
    const char* tempPayloadStr_cstr = tempPayloadStr.c_str();
    memcpy(&((char*)fullMessage)[sizeof(StBaseHeader)], &tempPayloadStr_cstr, totalPayloadSize);
    //free(header);
    return fullMessage;
}

string TextMsg::getPayloadString() {
    return m_payloadString;
}
