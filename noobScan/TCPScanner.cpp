//
//  TCPScanner.c
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "TCPScanner.h"

TCPScanner::TCPScanner(){
    this->openPorts.reserve(1000);
    this->variableScanTime = false;
}

TCPScanner::TCPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan){
    this->openPorts.reserve(1000);
    this->sleepTimer=newSleepTimer;
    this->timeoutTimer=newTimeoutTimer;
    this->variableScanTime=variableScan;
}

void TCPScanner::addOpenPorts(int newOpenPort){
    this->openPorts.emplace_back(newOpenPort);
    return;
}

void TCPScanner::addClosedPorts(int newClosedPort){
    this->closedPorts.emplace_back(newClosedPort);
    return;
}

// return vector of open ports
vector<int> TCPScanner::getOpenPorts(){
    return this->openPorts;
}

// output the open ports
void TCPScanner::printOpenPorts(){
    if(!this->openPorts.empty()){
        cout << "Open ports: \n";
        for(auto const & portValue: this->openPorts){
            cout << portValue << endl;
        }
    }
    return;
}

// runs a TCP scan on a single port
NoobCodes TCPScanner::runScan(int portNum, std::string IPToScan){
    
    // make a socket to scan ports
    int ourTCPSock = socket(AF_INET, SOCK_STREAM, 0);
    
    // if the socket wasn't unsuccessful
    if(ourTCPSock == -1){
        return NoobCodes::socketCreationError;
    }
    
    // create socket structure to check port
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

// runs TCP scan on multiple ports 
NoobCodes TCPScanner::runMultiScan(vector<unsigned> portNumbers, std::string IPToScan){
    
    // fd variables for modifying wait time
    fd_set ourFD;
    
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
            return NoobCodes::socketCreationErrorDGRAM;
        }
        
        socketToScan.sin_port = htons(nextPort);
    
        // if variable scan is on, wait a variable amount of time between each scan
        if(this->getVariableScanStatus()){
            //cout << "Variable scan is on\n";
            usleep(this->getSleepTimer() + generateNewSeed());
        }
        // if it's off, wait however long the user selected to wait between scans
        else{
            //cout << "Variable scan is off\n";
            usleep(this->getSleepTimer());
        }
        
        // set time out length variable to ensure we don't wait forever when checking a port, later
        struct timeval timeout;
        timeout.tv_sec=this->getTimeoutTimer();
        timeout.tv_usec=0;

        // ensure our TCP socket is not blocking (essential for timeout check). Here, fcntl sets the file descriptor flag of our TCP connecting socket to nonblock.
        fcntl(ourTCPSock, F_SETFL, O_NONBLOCK);
        
        // attempt to connect to the port (this would hang on some ports if the timeout system above was not used
        int checkConnect = connect(ourTCPSock, (sockaddr*)&socketToScan, sizeof(socketToScan));
        
        // determine if socket is open based on select, as the above call will be passed even if it's "hanging" until our select timeout passes, or a response is received. NOTE: some sites (like mine) will still ignore an attempt to connect as a security feature, and the below code will not result in output, even though it passes. This program is not made to bypass this, and will play nice with the response style of the IP.
        FD_ZERO(&ourFD);
        FD_SET(ourTCPSock, &ourFD);
        
        if(select(ourTCPSock+1, NULL, &ourFD, NULL, &timeout) == 1){
            int errorCheck;
            socklen_t errorLen = sizeof(errorCheck);
            getsockopt(ourTCPSock, SOL_SOCKET, SO_ERROR, &errorCheck, &errorLen);
            if(errorCheck==0){
                cout << "\tPort " << nextPort << " open\n";
                addOpenPorts(nextPort);
            }
            else{
                connectCheck(checkConnect, nextPort);
            }
        }
        
        // close the socket, and step to the next one if there are more left
        close(ourTCPSock);
    }
    
    return NoobCodes::success;
}

// check if connection was successful
void TCPScanner::connectCheck(int checkNum, unsigned int nextPort){
    
    // if connection denied, return that info
//    if(checkNum==-1){
    if(checkNum<0){
        addClosedPorts(nextPort);
        switch (errno) {
            case ECONNREFUSED:
                cout << "\tPort " << nextPort << " closed\n";
                break;
            case ETIMEDOUT:
                cout << "\tPort " << nextPort << " closed: timeout (perhaps try later)\n";
                break;
            case EACCES:
            case EPERM:
                cout << "\tPort " << nextPort << " closed: permissions issue\n";
                break;
            case EAGAIN:
                cout << "\tPort " << nextPort << " closed: nonblocking/busy, or routing cache issue\n";
                break;
            case ENETUNREACH:
                cout << "\tPort " << nextPort << " closed: network reachability issue\n";
                break;
            default:
                cout << "\tPort " << nextPort << " closed\n";
                break;
        }
    }
    
    // if the connection succeeded, add to open port list, close socket, and return success
    else{
        cout << "\tPort " << nextPort << " open\n";
        addOpenPorts(nextPort);
    }
    return;
}


// get sleep time
unsigned int TCPScanner::getSleepTimer(){
    return this->sleepTimer;
}

// get timeout time
unsigned int TCPScanner::getTimeoutTimer(){
    return this->timeoutTimer;
}

bool TCPScanner::getVariableScanStatus(){
    return this->variableScanTime;
}

//void debug(){
//    switch(errno){
//        case EBADF:
//            cout << "EBADF\n";
//            break;
//        case EDOM:
//            cout << "EDOM\n";
//            break;
//        case EINVAL:
//            cout << "EINVAL\n";
//            break;
//        case EISCONN:
//            cout << "EISCONN\n";
//            break;
//        case ENOPROTOOPT:
//            cout << "ENOPROTOOPT\n";
//            break;
//        case ENOTSOCK:
//            cout << "ENOTSOCK\n";
//            break;
//        default:
//            cout << "FUCK";
//    }
//        if(setsockopt(ourTCPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout))<0){
//            cout << "setsockopt fail\n";
//        }
//        struct sigaction stopSignal;
        
//        long nonBlock;
//        if((nonBlock = fcntl(ourTCPSock, F_GETFL, NULL))<0){
//            cout << "fcntl fucked you.\n";
//        }
//        nonBlock |= O_NONBLOCK;
//        if(fcntl(ourTCPSock, F_SETFL, nonBlock)<0){
//            cout << "setting nonblock FUCKED YOU.\n";
//        }
//}
