//
//  DestroyChatroomMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__DestroyChatroomMsg__
#define __MessageHierarchyP2P__DestroyChatroomMsg__

#include <iostream>
#include "BaseMessage.h"

class DestroyChatroomMsg: public BaseMessage {
public :
    DestroyChatroomMsg(string username, Direction dir, string chatRoomName, string payload);
    DestroyChatroomMsg(void* input);
    ~DestroyChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getDestroyPayload();
    
private:
    string m_destroyPayload;
    const static string m_prefixP2S;
    const static string m_prefixS2P;
    const static string m_S2P_ERR;
    const static string m_postfixP2S;
    const static string m_postFixS2P;
};


#endif /* defined(__MessageHierarchyP2P__DestroyChatroomMsg__) */
