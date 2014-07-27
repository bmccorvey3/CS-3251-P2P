//
//  UpdateRecipientsMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "UpdateRecipientsMsg.h"
#include <arpa/inet.h>

UpdateRecipientsMsg::UpdateRecipientsMsg(std::string username, Direction dir,
        		std::string chatRoomName, IPaddrStruct* ipAddr1, IPaddrStruct* ipAddr2)
				: BaseMessage(username, dir, chatRoomName)
{
		std::stringstream ss;
		m_ipAddr1
//		m_payloadString = ;
	    if(dir == Direction::P2S) {
	    	// "Peers updated."
	    	ss << "Peers updated.";
	    	m_payloadString = ss.str();
	        m_messageType = MessageType::UPDATE_P2S;
	        m_length = HEADER_LENGTH + m_payloadString.length();
	        m_code = "cups";
	    }
	    else if (dir == Direction::S2P) {
	    	// "New peers: " + updated primary IP address + "," + updated secondary IP address + "."
	    	ss << "New peers: " << updated primary IP address <<
	    			"," << updated secondary IP address << ".";
	        m_messageType = MessageType::UPDATE_S2P;
	        m_length = HEADER_LENGTH + s2pTotalPayloadSize;
	        m_code = "cusp";
	    }
	    else {
	        fprintf(stderr, "Incorrect direction in ChooseUsername!\n");
	    }
}

UpdateRecipientsMsg::~UpdateRecipientsMsg() {}

unsigned int UpdateRecipientsMsg::getLength() {
    return BaseMessage::l;
}

char* UpdateRecipientsMsg::getUsername() {
    return BaseMessage::user;
}

unsigned int UpdateRecipientsMsg::getSalt() {
    return BaseMessage::salt;
}

char* UpdateRecipientsMsg::getType() {
    return BaseMessage::type;
}

void* UpdateRecipientsMsg::getPayload() {
    return BaseMessage::payload;
}

Direction UpdateRecipientsMsg::getDirection() {
    Direction dir = Direction::ERROR;
    char peerServer[] = "upps";
    char serverPeer[] = "upsp";
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

std::string UpdateRecipientsMsg::getStringFromPayload(void* payload) {
    std::string *payload_pointer = static_cast<std::string*>(payload);
    std::string s = *payload_pointer;
    return s;
}

std::string UpdateRecipientsMsg::getPrimaryRecipients() {
    std::string primary = getStringFromPayload(BaseMessage::payload);
    unsigned long last = primary.find_last_of("New peers: ");
    unsigned long first = primary.find_first_of(",");
    return primary.substr(last+1,first-last-1);
}

std::string UpdateRecipientsMsg::getSecondaryRecipients() {
    std::string secondary = getStringFromPayload(BaseMessage::payload);
    unsigned long last = secondary.find_last_of(",");
    unsigned long first = secondary.find_first_of(".");
    return secondary.substr(last+1,first-last-1);
}
 
std::string UpdateRecipientsMsg::getIPaddrString(IPaddrStruct* ipAddr){
	unsigned long actualIP = ntohl(ipAddr->sin_addr.s_addr);
	std::stringstream ss;
	ss << int((actualIP&0xFF000000)>>24) << "."
		<< int((actualIP&0xFF0000)>>16) << "."
		<< int((actualIP&0xFF00)>>8) << "."
		<< int(actualIP&0xFF);
	return ss.str();
}

std::string UpdateRecipientsMsg::getPortString(IPaddrStruct* ipAddr){
	unsigned short actualPort = ntohs(ipAddr->sin_port);
	std::stringstream ss;
	ss << actualPort;
	return ss.str();
}

