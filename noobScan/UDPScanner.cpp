//
//  UDPScanner.cpp
//  noobScan
//
//  Created by John Atti on 4/26/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "UDPScanner.h"

UDPScanner::UDPScanner(){
    return;
}

NoobCodes UDPScanner::runScan(int portNum, std::string IPToScan){
    // make a socket to scan ports
    int ourTCPSock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // if the socket was unsuccessful
    if(ourTCPSock == -1){
        return NoobCodes::socketCreationError;
    }
    
    // create socket structure to check port
    struct sockaddr_in socketToScan;
    socketToScan.sin_family = AF_INET;
    socketToScan.sin_port = htons(portNum);
    
    // convert IP address accordingly
    if(!inet_pton(AF_INET, IPToScan.c_str(), &socketToScan.sin_addr)){
        std::cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    // check port

    //UDP reply: port open
    //ICMP unreachable: port is closed. ICMP is rate limited: might not get this reply.
    
    //no response: port is either open or filtered
    
    // typically more targeted scan, due to the time it takes
    
    
//    int checkConnect = connect(ourTCPSock, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // if connection denied, return that info
//    if(checkConnect==-1){
//        std::cout << "Port " << portNum << " closed\n";
//        return NoobCodes::portConnectionDenied;
//    }
    
    // if the connection succeeded, add to open port list, close socket, and return success
//    else{
//        //addOpenPorts(portNum);
//        std::cout << "Port " << portNum << " open\n";
//        // close the port
//        close(ourTCPSock);
//        return NoobCodes::portConnectionSuccess;
//    }
    return NoobCodes::success;
}
