//
//  main.cpp
//  TestingMessage
//
//  Created by Brent on 7/26/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include <iostream>
#include "BaseMessage.h"
#include "ChooseUsernameMsg.h"
#include "CreateChatroomMsg.h"
#include "DestroyChatroomMsg.h"
#include "EnterChatroomMsg.h"
#include "LeaveChatroomMsg.h"
#include "ListChatroomMsg.h"
#include "NotifyDroppedPeerMsg.h"
#include "TextMsg.h"
#include "UpdateRecipientsMsg.h"
#include <cassert>

using std::cout;
using std::endl;

int main() {
    
    std::cout<<"Hello World"<<std::endl;
    
//    CreateChatroomMsg crMsg("Brent", Direction::P2S,"Brent's Chatroom");
//    std::string createPayload = crMsg.getPayloadString();
    TextMsg* txtMsg = new TextMsg("Brent", "Chatroom", "This is a test message");
    std::string txtStr = txtMsg->getPayloadString();
    cout<< txtStr <<endl;
   // cout<< txtMsg->getLength()<<endl;
    cout<< "BaseMessage from known params:" << endl
        << BaseMessage::log(txtMsg)<<endl;
    TextMsg* msg = new TextMsg(txtMsg->getMessageStruct());
    cout << "Length of payload: " << txtMsg->getPayloadString().length() << endl
    << "Length of prefix str for TextMsg: " << txtMsg->m_prefixStr << endl;
    cout<< msg->getLength()<<endl;
    cout<<msg->getPayloadString()<<endl;
    

    cout<< "BaseMessage from void*:" << endl
        << BaseMessage::log(msg)<<endl;
    //BaseMessage::assertEquals(txtMsg,msg);
    
    return 0;
}