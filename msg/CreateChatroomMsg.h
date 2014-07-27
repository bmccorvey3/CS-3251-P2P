//
//  CreateChatroomMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__CreateChatroomMsg__
#define __MessageHierarchyP2P__CreateChatroomMsg__

#include <iostream>
#include "BaseMessage.h"

class CreateChatroomMsg : public BaseMessage {
public :
    CreateChatroomMsg(string username, Direction dir, string chatRoomName);
    CreateChatroomMsg(void* input);
    ~CreateChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getPayloadString();

    
private:
    const static string m_prefixP2S;
    const static string m_prefixS2P;
    const static string m_prefixS2P_ERR;
    const static string m_postfixP2S;
    const static string m_postFixS2P;
    const static string m_postfixS2P_ERR;
    static const size_t p2sTotalPayloadSize = sizeof(m_payloadString)+ sizeof(m_prefixP2S) +
                                                sizeof(m_postfixP2S);
    static const size_t s2pTotalPayloadSize = sizeof(m_payloadString) + sizeof(m_prefixS2P)
                                                + sizeof(m_postFixS2P);
    static const size_t s2pErrTotalPayloadSize = sizeof(m_payloadString) +
                            + sizeof(m_prefixS2P_ERR) + sizeof(m_postfixS2P_ERR);
    
    typedef struct __attribute__((packed)) FULL_MESSAGE_P2S {
        StBaseHeader stBaseHeader;
        char payload[p2sTotalPayloadSize];
    } FullMessageP2S;
    typedef struct __attribute__((packed)) FULL_MESSAGE_S2P {
        StBaseHeader stBaseHeader;
        char payload[s2pTotalPayloadSize];
    } FullMessageS2P;
    typedef struct __attribute__((packed)) FULL_MESSAGE_S2P_ERR {
        StBaseHeader stBaseHeader;
        char payload[s2pErrTotalPayloadSize];
    } FullMessageS2PErr;

};


#endif /* defined(__MessageHierarchyP2P__CreateChatroomMsg__) */
