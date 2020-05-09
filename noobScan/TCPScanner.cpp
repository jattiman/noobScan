//
//  TCPScanner.c
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "TCPScanner.h"

TCPScanner::TCPScanner(){
    //this->openPorts.clear();
    this->openPorts.reserve(1000);
    //this->closedPorts.clear();
}

void TCPScanner::addOpenPorts(int newOpenPort){
    this->openPorts.emplace_back(newOpenPort);
    return;
}

vector<int> TCPScanner::getOpenPorts(){
    return this->openPorts;
}

void TCPScanner::printOpenPorts(){
    for(auto const & portValue: this->openPorts){
        cout << portValue << endl;
    }
    return;
}

NoobCodes TCPScanner::runScan(int portNum, std::string IPToScan){
    
    // make a socket to scan ports
    int ourTCPSock = socket(AF_INET, SOCK_STREAM, 0);
    
    // if the socket wasn't unsuccessful
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
        cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    // check port
    int checkConnect = connect(ourTCPSock, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // if connection denied, return that info
    if(checkConnect==-1){
        cout << "TCP Port " << portNum << " closed\n";
        close(ourTCPSock);
        return NoobCodes::portConnectionDenied;
    }
    
    // if the connection succeeded, add to open port list, close socket, and return success
    else{
        cout << "TCP Port " << portNum << " open\n";
        // close the port
        close(ourTCPSock);
        addOpenPorts(portNum);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}

// scan multiple ports at once
NoobCodes TCPScanner::runMultiScan(vector<int> portNumbers, std::string IPToScan){
    
    // placeholder for scanning socket
    int ourTCPSock = 0;
    
    // create socket structure to check ports
    struct sockaddr_in socketToScan;
    socketToScan.sin_family = AF_INET;
    
    // convert IP address accordingly
    if(!inet_pton(AF_INET, IPToScan.c_str(), &socketToScan.sin_addr)){
        // If there's an issue with binding, return error code accordingly
        cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    // run a loop to iterate through the ports
    for(auto const & nextPort: portNumbers){
        
        // make a socket to scan ports
        ourTCPSock = socket(AF_INET, SOCK_STREAM, 0);
        
        // if the socket wasn't unsuccessful
        if(ourTCPSock == -1){
            return NoobCodes::socketCreationError;
        }
        
        socketToScan.sin_port = htons(nextPort);
    
        // check port
        int checkConnect = connect(ourTCPSock, (sockaddr*)&socketToScan, sizeof(socketToScan));
        
        // if connection denied, return that info
        if(checkConnect==-1){
            cout << "Port " << nextPort << " closed\n";
        }
        
        // if the connection succeeded, add to open port list, close socket, and return success
        else{
            cout << "Port " << nextPort << " open\n";
            // close the port
            addOpenPorts(nextPort);
        }
        
        close(ourTCPSock);
    }
    
    return NoobCodes::success;
}
