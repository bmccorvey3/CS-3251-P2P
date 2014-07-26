//
//  ChooseUsernameMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "ChooseUsernameMsg.h"

/*ChooseUsernameMsg::ChooseUsernameMsg(unsigned int length, char* username,
                                     unsigned int salt, char* type,
                                     void* payload) : BaseMessage(length,
                                                username,salt,type,payload) {}*/

ChooseUsernameMsg::ChooseUsernameMsg(Direction dir, MessageType type, string username)  {
    
    m_dir = dir;
    m_username = username;
    std::stringstream ss;
    if(dir == Direction::P2S && type == MessageType::CHOOSE_P2S) {
        ss<<"My username:" << username;
        m_payload = ss.str();
    }
    else if (dir == Direction::S2P && type == MessageType::CHOOSE_S2P) {
        ss<<"Hello, " <<username;
        m_payload = ss.str();
    }
}

ChooseUsernameMsg::~ChooseUsernameMsg() {}

unsigned int ChooseUsernameMsg::getLength() {
    return m_length;
}

string ChooseUsernameMsg::getUsername() {
    return m_username;
}

unsigned int ChooseUsernameMsg::getSalt() {
    return m_salt;
}

string ChooseUsernameMsg::getType() {
    return m_string_type;
}

string ChooseUsernameMsg::getPayload() {
    return m_payload;
}

/*static ChooseUsernameMsg* parseMsg(void* msg) {
    Direction dir;
    std::string *msg_ptr = static_cast<std::string*>(msg);
    std::string str_msg = *msg_ptr;
    std::string payload = str_msg.substr(BaseMessage::HEADER_LENGTH);
    if(str_msg.find("cups") != std::string::npos) {
        dir = Direction::P2S;
    }
    else if(str_msg.find("cusp") != std::string::npos) {
        dir = Direction::S2P;
    }
    unsigned long last = payload.find_last_of("My username: ");
    std::string user = payload.substr(last+1);
    char* username = &user[0];
    
    return new ChooseUsernameMsg(dir,username);
}


void* ChooseUsernameMsg::sendMsg() {
    
    
}
*/

void ChooseUsernameMsg::parseSpecificMessage(void* msg){
    BaseMessage* baseMessage  = new BaseMessage(msg);
    // getBaseMessage(msg);
    void* payload = baseMessage->getPayloadPtr();
    std::string *payload_ptr = static_cast<std::string*>(payload);
    std::string payloadStr = *payload_ptr;
    std::string myUsername = "";
    if(baseMessage->getDirection() == Direction::P2S) {
       myUsername = "My username:";
    }
    else if(baseMessage->getDirection() == Direction::S2P) {
        myUsername = "Hello, ";
    }
    std::string checkStr = payloadStr.substr(0, sizeof(myUsername));
    if (myUsername != checkStr){
        fprintf(stderr, "Incorrect payload in choose username message: %s", checkStr.c_str());
    }
    unsigned char* msg_char = static_cast<unsigned char*>(msg);
    memcpy(&m_username,  &msg_char[sizeof(myUsername)-1], sizeof(m_username)-1);
    //TODO check if sizeof incorporates null terminating character
    //Make sure to subtract 1 from the sizeof
}

/* in peer code
ChooseUsernameMsg* msg = new ChooseUsernameMsg("Bob");
void* msgToSend = msg->getMessageStruct();
 
*/

/*Direction ChooseUsernameMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "cups";
    char serverPeer[] = "cusp";
    for(int i = 0; i<(sizeof(BaseMessage::type)/BaseMessage::type[0]); ++i) {
        if(peerServer[i] == *(BaseMessage::type + i)) {
            dir = Direction::P2S;
        }
        else if(serverPeer[i] == *(BaseMessage::type + i)) {
            dir = Direction::S2P;
        }
    }
    return dir;
}

std::string ChooseUsernameMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string ChooseUsernameMsg::getUsernameP2S() {
    std::string username = getStringFromPayload(BaseMessage::payload);
    unsigned long last = username.find_last_of("My username: ");
    return username.substr(last+1);
}

std::string ChooseUsernameMsg::getUsernameS2P() {
    std::string username = getStringFromPayload(BaseMessage::payload);
    unsigned long last = username.find_last_of("Hello, ");
    return username.substr(last+1);
}*/