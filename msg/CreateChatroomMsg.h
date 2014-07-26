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
    //CreateChatroomMsg(unsigned int length, char* username, unsigned int salt,
     //       char* type, void* payload);
    CreateChatroomMsg(Direction dir, MessageType type, string chatRoomName);
    ~CreateChatroomMsg(); //TODO delete pointers
    unsigned int getLength();
    string getUsername();
    unsigned int getSalt();
    string getType();
    string getPayload();
    
    void parseSpecificMessage(void* msg);
    
    /*
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getChatRoomFromPayloadP2S();
    std::string getChatRoomFromPayloadS2P();
    */
};


#endif /* defined(__MessageHierarchyP2P__CreateChatroomMsg__) */
