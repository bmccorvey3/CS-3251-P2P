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
		m_ipAddr1 = getIPaddrString(ipAddr1);
		m_ipAddr2 = getIPaddrString(ipAddr2);
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
	    	ss << "New peers: " << getIPaddrString(ipAddr1) <<
	    			"," << getIPaddrString(ipAddr1) << ".";
	    	m_payloadString = ss.str();
	        m_messageType = MessageType::UPDATE_S2P;
	        m_length = HEADER_LENGTH + m_payloadString.length();
	        m_code = "cusp";
	    }
	    else {
	        fprintf(stderr, "Incorrect direction in ChooseUsername!\n");
	    }
}

//UpdateRecipientsMsg::UpdateRecipientsMsg(std::string username, Direction dir,
//        		std::string chatRoomName, IPaddrStruct* ipAddr1, IPaddrStruct* ipAddr2)
//				: BaseMessage(username, dir, chatRoomName)
//{
//	if (m_dir == Direction::P2S){
//			// "Peers updated."
//			m_ipAddr1 = nullptr;
//			m_ipAddr2 = nullptr;
//		} else if (m_dir == Direction::S2P){
//			// "Removing peer " + IP address + "."
//			m_ipAddr1 = m_payloadString.substr(sizeof("Removing peer "),m_payloadString.length()-1);
//			m_ipAddr2 = nullptr;
//		} else if (m_dir == Direction::ERROR) {
//			// "You have been knocked off."
//			m_ipAddr1 = nullptr;
//			m_ipAddr2 = nullptr;
//		} else {
//			fprintf(stderr, "Invalid direction in creating NotifyDroppedPeerMsg");
//		}
//}

UpdateRecipientsMsg::UpdateRecipientsMsg(void* input) : BaseMessage(input){
	if (m_dir == Direction::P2S){
			// "Peers updated."
			m_ipAddr1 = nullptr;
			m_ipAddr2 = nullptr;
		} else if (m_dir == Direction::S2P){
			// "Removing peer " + IP address + "."
			m_ipAddr1 = m_payloadString.substr(sizeof("Removing peer "),m_payloadString.length()-1);
			m_ipAddr2 = nullptr;
		} else if (m_dir == Direction::ERROR) {
			// "You have been knocked off."
			m_ipAddr1 = nullptr;
			m_ipAddr2 = nullptr;
		} else {
			fprintf(stderr, "Invalid direction in creating NotifyDroppedPeerMsg");
		}
}

UpdateRecipientsMsg::~UpdateRecipientsMsg() {}


 
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

std::string NotifyDroppedPeerMsg::getIPaddrString(IPaddrStruct* ipAddr){
	unsigned long actualIP = ntohl(ipAddr->sin_addr.s_addr);
	std::stringstream ss;
	ss << int((actualIP&0xFF000000)>>24) << "."
		<< int((actualIP&0xFF0000)>>16) << "."
		<< int((actualIP&0xFF00)>>8) << "."
		<< int(actualIP&0xFF);
	return ss.str();
}

std::string NotifyDroppedPeerMsg::getPortString(IPaddrStruct* ipAddr){
	unsigned short actualPort = ntohs(ipAddr->sin_port);
	std::stringstream ss;
	ss << actualPort;
	return ss.str();
}

IPaddrStruct* getIPaddrFromStr(std::string inputIp){
	int posOfColon = inputIp.find(":");
	std::string actualIPstr = inputIp.substr(0,posOfColon-1);
	IPaddrStruct* ipAddr = malloc(sizeof(IPaddr));
	memset(ipAddr, 0 ,sizeof(ipAddr));
	inet_pton(AF_INET, inputIp.c_str(), &(ipAddr->sin_addr));
	ipAddr->sin_family = AF_INET;
	if (inputIp.length() > posOfColon && posOfColon != std::string::npos){
		std::string actualPort = inputIp.substr(posOfColon+1);
			ipAddr->sin_port = atoi(actualPort.c_str());
	}
	return ipAddr;
}

