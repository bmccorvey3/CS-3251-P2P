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

class LeaveChatroomMsg : public BaseMessage {
    public :
    LeaveChatroomMsg(unsigned int length, char* username, unsigned int salt,
            char* type, void* payload);
    ~LeaveChatroomMsg();
    unsigned int getLength();
    char* getUsername();
    unsigned int getSalt();
    char* getType();
    void* getPayload();
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getUserNameFromPayload();
    std::string getChatRoomFromPayload();
};


#endif /* defined(__MessageHierarchyP2P__LeaveChatroomMsg__) */
