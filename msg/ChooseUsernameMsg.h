//
//  ChooseUsernameMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__ChooseUsernameMsg__
#define __MessageHierarchyP2P__ChooseUsernameMsg__

#include <iostream>
#include "BaseMessage.h"

class ChooseUsernameMsg : public BaseMessage {
public:
    ChooseUsernameMsg(string username, Direction dir, string payload);
    ChooseUsernameMsg(void* input);
    ~ChooseUsernameMsg(); //TODO delete pointers
    
    void* getMessageStruct();

    string getPayloadString();
    string getChooseString();
    string getPrefixP2S();
    string getPrefixS2P();
    
private:
    string m_choosePayload;
    const static string m_prefixP2S;
    const static string m_prefixS2P;
    static const size_t p2sTotalPayloadSize = sizeof(m_choosePayload)+ sizeof(m_prefixP2S);
    static const size_t s2pTotalPayloadSize = sizeof(m_choosePayload) + sizeof(m_prefixS2P);
    typedef struct __attribute__((packed)) FULL_MESSAGE_P2S {
        StBaseHeader stBaseHeader;
        char payload[p2sTotalPayloadSize];
    } FullMessageP2S;
    typedef struct __attribute__((packed)) FULL_MESSAGE_S2P {
        StBaseHeader stBaseHeader;
        char payload[s2pTotalPayloadSize];
    } FullMessageS2P;
};


#endif /* defined(__MessageHierarchyP2P__ChooseUsernameMsg__) */
