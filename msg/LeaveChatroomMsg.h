//
//  LeaveChatroomMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__LeaveChatroomMsg__
#define __MessageHierarchyP2P__LeaveChatroomMsg__

#include <iostream>
#include "BaseMessage.h"

class LeaveChatroomMsg: public BaseMessage {
public :
    LeaveChatroomMsg(string username, Direction dir, string chatRoomName, string payload);
    LeaveChatroomMsg(void* input);
    ~LeaveChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getleavePayload();
    
private:
    string m_leavePayload;
    const static string m_prefixS2P;
    const static string m_postfixP2S;
    const static string m_postFixS2P;
};

#endif /* defined(__MessageHierarchyP2P__LeaveChatroomMsg__) */
