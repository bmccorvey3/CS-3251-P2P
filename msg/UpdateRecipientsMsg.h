//
//  UpdateRecipientsMsg.h
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#ifndef __MessageHierarchyP2P__UpdateRecipientsMsg__
#define __MessageHierarchyP2P__UpdateRecipientsMsg__

#include <iostream>
#include "BaseMessage.h"

class UpdateRecipientsMsg : public BaseMessage {
    public :
    UpdateRecipientsMsg(unsigned int length, char* username, unsigned int salt,
            char* type, void* payload);
    ~UpdateRecipientsMsg(); //TODO delete pointers
    unsigned int getLength();
    char* getUsername();
    unsigned int getSalt();
    char* getType();
    void* getPayload();
    Direction getDirection();
    std::string getStringFromPayload(void*);
    std::string getPrimaryRecipients();
    std::string getSecondaryRecipients();
};


#endif /* defined(__MessageHierarchyP2P__UpdateRecipientsMsg__) */
