//
//  BaseMessage.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__BaseMessage__
#define __MessageHierarchyP2P__BaseMessage__

#include <iostream>

enum class Direction {
    P2S,
    S2P,
    TEXT,
    NONE
};

class BaseMessage {
public:
    BaseMessage();
    BaseMessage(unsigned int length, char* username, unsigned int salt,
                char* type, void* payload) : l{length}, user{username},
                salt{salt}, type{type}, payload{payload}{}
    
    unsigned int l;
    char* user;
    unsigned int salt;
    char* type;
    void* payload;
    
    virtual ~BaseMessage() {}
    virtual unsigned int getLength() = 0;
    virtual char* getUsername() = 0;
    virtual unsigned int getSalt() = 0;
    virtual char* getType() = 0;
    virtual void* getPayload() = 0;
    virtual Direction getDirection() = 0;
    virtual std::string getStringFromPayload(void*) = 0;
    
    
};

#endif /* defined(__MessageHierarchyP2P__BaseMessage__) */
