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

typedef struct sockaddr_in IPaddrStruct;
//    struct sockaddr_in {
//        short            sin_family;   // e.g. AF_INET, AF_INET6
//        unsigned short   sin_port;     // e.g. htons(3490)
//        struct in_addr   sin_addr;     // see struct in_addr, below
//        char             sin_zero[8];  // zero this if you want to
//    };

/*
 * The actual IP address
 */
typedef struct in_addr IPaddr;
//    struct in_addr {
//        unsigned long s_addr; // load with inet_pton()
//    };


class UpdateRecipientsMsg: public BaseMessage {
    public :
    UpdateRecipientsMsg(Direction dir, IPaddrStruct ip_struct, string payload);
    UpdateRecipientsMsg(void* input);
    ~UpdateRecipientsMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getUpdatePayload();
    
private:
    string m_updatePayload;
    const static string m_P2S;
    const static string m_prefixS2P;
    const static string m_midfixS2P;
    const static string m_postFixS2P;
};

#endif /* defined(__MessageHierarchyP2P__UpdateRecipientsMsg__) */
