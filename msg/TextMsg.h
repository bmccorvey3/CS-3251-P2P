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
    TextMsg(unsigned int length, char* username, unsigned int salt,
            char* type, void* payload);
    ~TextMsg();
    unsigned int getLength();
    char* getUsername();
    unsigned int getSalt();
    char* getType();
    void* getPayload();
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getPayloadText();
};

#endif /* defined(__MessageHierarchyP2P__TextMsg__) */
