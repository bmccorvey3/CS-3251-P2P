//
//  EnterChatroomMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__EnterChatroomMsg__
#define __MessageHierarchyP2P__EnterChatroomMsg__

#include <iostream>
#include "BaseMessage.h"

class EnterChatroomMsg: public BaseMessage {
public :
    EnterChatroomMsg(string username, Direction dir, string chatRoomName, string payload);
    EnterChatroomMsg(void* input);
    ~EnterChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getEnterPayload();
    
private:
    string m_enterPayload;
    const static string m_prefixS2P;
    const static string m_S2P_ERR;
    const static string m_postfixP2S;
    const static string m_postFixS2P;
};



#endif /* defined(__MessageHierarchyP2P__EnterChatroomMsg__) */
