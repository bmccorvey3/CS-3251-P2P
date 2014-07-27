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

typedef struct sockaddr_in IPaddrStruct;
//   struct sockaddr_in {
//       short            sin_family;   // e.g. AF_INET, AF_INET6
//       unsigned short   sin_port;     // e.g. htons(3490)
//       struct in_addr   sin_addr;     // see struct in_addr, below
//       char             sin_zero[8];  // zero this if you want to
//   };
//
///
//  The actual IP address
// /
typedef struct in_addr IPaddr;
//    struct in_addr {
//        unsigned long s_addr; // load with inet_pton()
//    };
//

class NotifyDroppedPeerMsg: public BaseMessage {
public :
    NotifyDroppedPeerMsg(std::string username, Direction dir,
    		std::string chatRoomName, IPaddrStruct* ipAddrOfInterest);
    NotifyDroppedPeerMsg(void* input);
    ~NotifyDroppedPeerMsg(); //TODO delete pointers
    
    void* getMessageStruct();
    
    string getNotifyPayload();
    
    static std::string getIPaddrString(IPaddrStruct* ipAddr);
    static std::string getPortString(IPaddrStruct* ipAddr);
    IPaddrStruct* getDroppedPeerIPaddr();
    IPaddrStruct* getIPaddrFromStr(std::string inputIp);
    std::string getPayloadString();

private:
    string m_notifyPayload;
    const static std::string m_prefixP2S;
    const static std::string m_prefixS2P;
    const static std::string m_midfixP2S;
    const static std::string m_S2P_ERR;
    const static std::string m_postfixP2S;
    const static std::string m_postFixS2P;
    std::string m_droppedPeer;
};

#endif /* defined(__MessageHierarchyP2P__NotifyDroppedPeerMsg__) */
