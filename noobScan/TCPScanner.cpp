//
//  TCPScanner.c
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "TCPScanner.h"

TCPScanner::TCPScanner(){
    this->openPorts.clear();
    this->closedPorts.clear();
}

void TCPScanner::addOpenPorts(int newOpenPort){
    this->openPorts.push_back(newOpenPort);
}

std::vector<int> TCPScanner::getOpenPorts(){
    return this->openPorts;
}

void TCPScanner::printOpenPorts(){
    for(auto const & portValue: this->openPorts){
        std::cout << portValue << std::endl;
    }
    return;
}

NoobCodes TCPScanner::runScan(int portNum, std::string IPToScan){
    // make a socket to scan ports
    int ourTCPSock = socket(AF_INET, SOCK_STREAM, 0);
    
    // if the socket was unsuccessful
    if(ourTCPSock == -1){
        return NoobCodes::socketCreationError;
    }
    
    // create socket structure to check port
    // TODO: figure out if socketToScan needs to be a struct
    struct sockaddr_in socketToScan;
    socketToScan.sin_family = AF_INET;
    socketToScan.sin_port = htons(portNum);
    
    // convert IP address accordingly
    if(!inet_pton(AF_INET, IPToScan.c_str(), &socketToScan.sin_addr)){
        std::cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    // check port
    int checkConnect = connect(ourTCPSock, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // if connection denied, return that info
    if(checkConnect==-1){
        std::cout << "Port " << portNum << " closed\n";
        return NoobCodes::portConnectionDenied;
    }
    
    // if the connection succeeded, add to open port list, close socket, and return success
    else{
        addOpenPorts(portNum);
        std::cout << "Port " << portNum << " open\n";
        // close the port
        close(ourTCPSock);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}
