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
    CreateChatroomMsg(string username, Direction dir, string chatRoomName, string payload);
    CreateChatroomMsg(void* input);
    ~CreateChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getChoosePayload();
    
private:
    string m_createPayload;
    const static string m_prefixP2S;
    const static string m_prefixS2P;
    const static string m_prefixS2P_ERR;
    const static string m_postfixP2S;
    const static string m_postFixS2P;
    const static string m_postfixS2P_ERR;
};


#endif /* defined(__MessageHierarchyP2P__CreateChatroomMsg__) */
