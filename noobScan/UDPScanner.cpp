//
//  UDPScanner.cpp
//  noobScan
//
//  Created by John Atti on 4/26/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "UDPScanner.h"

UDPScanner::UDPScanner(){
    this->ourSleepTimer=getSleepTimer();
    return;
}

NoobCodes UDPScanner::runScan(int portNum, std::string IPToScan){
    
    // UDP is typically more targeted scan, due to the time it takes
    
    // make a socket to scan ports
    int ourUDPSock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // if the socket was unsuccessful
    if(ourUDPSock == -1){
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
    
    string testString="test\nhelp\n";
    
    // check port (sendto returns ssize_t and not int)
    ssize_t portCheck=sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // sleep while we wait for port response
    //usleep(this->ourSleepTimer);
    
    // return status of scan
    if(portCheck<0){
        // if there is a connection error, the port is likely closed
        // review error status, and report why it's closed
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << portNum << " closed: nonblocking port blocked by request\n";
        }
        else if(errno == EBADF){
            cout << "Port " << portNum << " file descriptor issue\n";
        }
        else if(errno == EFAULT){
            cout << "Port " << portNum << " rejected userspace address\n";
        }
        else if(errno == EINVAL){
            cout << "Port " << portNum << " received invalid argument\n";
        }
        else if(errno == EMSGSIZE){
            cout << "Port " << portNum << " issue: message size error \n";
        }
        else if(errno == ENOBUFS){
            cout << "Port " << portNum << " issue: output queue full\n";
        }
        else if(errno == EOPNOTSUPP){
            cout << "Port " << portNum << " had issue with flags\n";
        }
        else{
            cout << "Port " << portNum << " likely closed\n";
            addPortList(portNum, this->closedPorts);
        }
        return NoobCodes::portConnectionDenied;
    }
    //ICMP unreachable: port is closed. ICMP is rate limited: might not get this reply.
    //UDP reply: port open
    //no response: port is either open or filtered
    else{
        // if there is a response, or no response, the port is likely open
        cout << "Port " << portNum << " open / filtered\n";
        addPortList(portNum, this->openPorts);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}

NoobCodes UDPScanner::runMultiScan(vector<unsigned> portNumbers, string IPToScan){
    
    // UDP is typically more targeted scan, due to the time it takes
    
    // make a socket to scan ports
    int ourUDPSock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // if the socket was unsuccessful
    if(ourUDPSock == -1){
        return NoobCodes::socketCreationError;
    }
    
    // create socket structure to check port
    struct sockaddr_in socketToScan;
    socketToScan.sin_family = AF_INET;
    // TODO: change this placeholder
    socketToScan.sin_port = htons(portNumbers[0]);
    
    // convert IP address accordingly
    if(!inet_pton(AF_INET, IPToScan.c_str(), &socketToScan.sin_addr)){
        std::cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    string testString="test\nhelp\n";
    
    // check port (sendto returns ssize_t and not int)
    ssize_t portCheck=sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // sleep while we wait for port response
    //usleep(this->ourSleepTimer);
    
    // return status of scan
    if(portCheck<0){
        // if there is a connection error, the port is likely closed
        // TODO: change these placeholders
        cout << "Port " << portNumbers[0] << " likely closed\n";
        addPortList(portNumbers[0], this->closedPorts);
        return NoobCodes::portConnectionDenied;
    }
    //ICMP unreachable: port is closed. ICMP is rate limited: might not get this reply.
    //UDP reply: port open
    //no response: port is either open or filtered
    else{
        // if there is a response, or no response, the port is likely open
        // TODO: change these placeholders
        cout << "Port " << portNumbers[0] << " open / filtered\n";
        addPortList(portNumbers[0], this->openPorts);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}
