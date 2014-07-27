//
//  NotifyDroppedPeerMsg.cpp
//  MessageHierarchyP2P
//
//  Created by Brent on 7/19/14.
//  Copyright (c) 2014 Brent. All rights reserved.
//

#include "NotifyDroppedPeerMsg.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

const string NotifyDroppedPeerMsg::m_prefixP2S = "Peer ";
const string NotifyDroppedPeerMsg::m_midfixP2S = ":";
const string NotifyDroppedPeerMsg::m_postfixP2S = " dropped.";
const string NotifyDroppedPeerMsg::m_prefixS2P = "Removing peer ";
const string NotifyDroppedPeerMsg::m_postFixS2P = ".";
const string NotifyDroppedPeerMsg::m_S2P_ERR = "You have been knocked off.";

NotifyDroppedPeerMsg::NotifyDroppedPeerMsg(std::string username, Direction dir,
		std::string chatRoomName, IPaddrStruct* ipAddrOfInterest) :
		BaseMessage(username, dir, chatRoomName) {
	std::string ipAddr = getIPaddrString(ipAddrOfInterest);
	std::string port = getPortString(ipAddrOfInterest);
	m_droppedPeer = ipAddr + ":" + port;
	if (dir == Direction::P2S){
		// "Peer " + IP address + ":" portNumber + " dropped."
		m_messageType = MessageType::NOTIFY_P2S;
		m_code = "pdps";
		m_length = HEADER_LENGTH + sizeof(m_prefixP2S) + sizeof(m_midfixP2S) + sizeof(m_postfixP2S)
				+ ipAddr.length() + port.length();
	} else if (dir == Direction::S2P){
		// "Removing peer " + IP address + "."
		m_messageType = MessageType::NOTIFY_S2P;
		m_code = "pdsp";
		m_length = HEADER_LENGTH + sizeof(m_prefixS2P) + sizeof(m_postFixS2P) + ipAddr.length();
	} else if (dir == Direction::ERROR) {
		// "You have been knocked off."
		m_messageType = MessageType::NOTIFY_S2P;
		m_code = "pksp";
		m_length = HEADER_LENGTH + sizeof(m_S2P_ERR);
	} else {
		fprintf(stderr, "Invalid direction in creating NotifyDroppedPeerMsg");
	}
}

NotifyDroppedPeerMsg::NotifyDroppedPeerMsg(void* input) : BaseMessage(input){
    char* textPayload = (char*) malloc(m_length);
    memcpy(textPayload, &((char*)input)[HEADER_LENGTH], m_length);
    std::string tempTextPayload(textPayload);
    if(tempTextPayload.find(m_prefixStr) == POS_TXT_PREFIX){
        m_textPayload = std::string(textPayload);
        // cut down on string to what we actuall want
        m_textPayload = m_textPayload.substr(POS_TXT_PREFIX+m_prefixStr.size());
        // TODO deallocate m_textPayload in destructor
    } else {
        fprintf(stderr, "Couldn't find 'txt:' in text payload\n");
        // TODO add log information
    }
    // free textPayload; // TODO do I need this?
}

NotifyDroppedPeerMsg::~NotifyDroppedPeerMsg() {}

IPaddrStruct* getDroppedPeerIPaddr(){

	return nullptr;
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
	std::string actualPort = inputIp.substr(posOfColon+1);
	IPaddrStruct* ipAddr = malloc(sizeof(IPaddr));
	memset(ipAddr, 0 ,sizeof(ipAddr));
	inet_pton(AF_INET, inputIp.c_str(), &(ipAddr->sin_addr));
	ipAddr->sin_family = AF_INET;
	ipAddr->sin_port = atoi(actualPort.c_str());
	return ipAddr;
}
