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
#define LENGTH_OF_USERNAME 16

class ChooseUsernameMsg : public BaseMessage {
    public :
    //ChooseUsernameMsg(unsigned int length, char* username, unsigned int salt,
    //        char* type, void* payload);
    ChooseUsernameMsg(Direction dir, MessageType type, string username);
    ~ChooseUsernameMsg(); //TODO delete pointers
    
    unsigned int getLength();
    string getUsername();
    unsigned int getSalt();
    string getType();
    string getPayload();
    
    void parseSpecificMessage(void* msg);
    
   // static ChooseUsernameMsg* parseMsg(void* msg);
    
    
 //   void* sendMsg();
    
   /* Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getUsernameP2S();
    std::string getUsernameS2P();*/
};


#endif /* defined(__MessageHierarchyP2P__ChooseUsernameMsg__) */
