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
public :
    
    ChooseUsernameMsg(string username, Direction dir, string payload);
    ChooseUsernameMsg(void* input);
    ~ChooseUsernameMsg(); //TODO delete pointers
    
    void* getMessageStruct();

    string getChoosePayload();
    string getPrefixP2S();
    string getPrefixS2P();
    
private:
    string m_choosePayload;
    const static string m_prefixP2S;
    const static string m_prefixS2P;
 
};


#endif /* defined(__MessageHierarchyP2P__ChooseUsernameMsg__) */
