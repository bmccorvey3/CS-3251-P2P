//
//  TextMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__TextMsg__
#define __MessageHierarchyP2P__TextMsg__

#include <iostream>
#include "BaseMessage.h"

class TextMsg : public BaseMessage {
public :
    TextMsg(std::string username, std::string chatRoomName, std::string payload);
    TextMsg(void* input);
    ~TextMsg(); //TODO delete pointers

    void* getMessageStruct();
    
    std::string getTextPayload();
    string getPayloadString();
    
private:
    std::string m_textPayload;
    const static std::string m_prefixStr;
};

#endif /* defined(__MessageHierarchyP2P__TextMsg__) */
