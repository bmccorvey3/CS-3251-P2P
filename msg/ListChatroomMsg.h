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
    ListChatroomMsg(Direction dir, string listChatrooms, string payload);
    ListChatroomMsg(void* input);
    ~ListChatroomMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getListPayload();
    
private:
    string m_listPayload;
    const static string m_P2S;
    const static string m_prefixS2P;
    const static string m_postFixS2P;
};



#endif /* defined(__MessageHierarchyP2P__ListChatroomMsg__) */
