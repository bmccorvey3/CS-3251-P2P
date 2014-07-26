//
//  NotifyDroppedPeerMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__NotifyDroppedPeerMsg__
#define __MessageHierarchyP2P__NotifyDroppedPeerMsg__

#include <iostream>
#include "BaseMessage.h"

class NotifyDroppedPeerMsg : public BaseMessage {
    public :
    NotifyDroppedPeerMsg(unsigned int length, char* username, unsigned int salt,
            char* type, void* payload);
    ~NotifyDroppedPeerMsg(); //TODO delete pointers
    unsigned int getLength();
    char* getUsername();
    unsigned int getSalt();
    char* getType();
    void* getPayload();
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getDroppedIpP2S();
    std::string getDroppedIpS2P();
    std::string getDroppedPortP2S();
};


#endif /* defined(__MessageHierarchyP2P__NotifyDroppedPeerMsg__) */
