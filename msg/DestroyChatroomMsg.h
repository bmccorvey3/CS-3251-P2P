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

class DestroyChatroomMsg : public BaseMessage {
    public :
    DestroyChatroomMsg(unsigned int length, char* username, unsigned int salt,
            char* type, void* payload);
    ~DestroyChatroomMsg(); //TODO delete pointers
    unsigned int getLength();
    string getUsername();
    unsigned int getSalt();
    string getType();
    string getPayload();
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getDestroyedChatRoomP2S();
    std::string getDestroyedChatRoomS2P();
};


#endif /* defined(__MessageHierarchyP2P__DestroyChatroomMsg__) */
