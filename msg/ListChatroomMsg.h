//
//  ListChatroomMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__ListChatroomMsg__
#define __MessageHierarchyP2P__ListChatroomMsg__

#include <iostream>
#include "BaseMessage.h"

class ListChatroomMsg: public BaseMessage {
public :
    ListChatroomMsg(Direction dir, string listChatrooms);
    ListChatroomMsg(void* input);
    ~ListChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getListPayload();
    string getPayloadString();

    
private:
    string m_listPayload;
    const static string m_P2S;
    const static string m_prefixS2P;
    const static string m_postFixS2P;
    
    static const size_t p2sTotalPayloadSize = sizeof(m_P2S);
    static const size_t s2pTotalPayloadSize = sizeof(m_prefixS2P) + sizeof(m_postFixS2P);
    
    typedef struct __attribute__((packed)) FULL_MESSAGE_P2S {
        StBaseHeader stBaseHeader;
        char payload[p2sTotalPayloadSize];
    } FullMessageP2S;
    typedef struct __attribute__((packed)) FULL_MESSAGE_S2P {
        StBaseHeader stBaseHeader;
        char payload[s2pTotalPayloadSize];
    } FullMessageS2P;

    
    
};



#endif /* defined(__MessageHierarchyP2P__ListChatroomMsg__) */
