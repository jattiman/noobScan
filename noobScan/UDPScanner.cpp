//
//  UDPScanner.cpp
//  noobScan
//
//  Created by John Atti on 4/26/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "UDPScanner.h"
#define MAXRETURN 1024

UDPScanner::UDPScanner(){
    this->ourSleepTimer=getSleepTimer();
    return;
}

NoobCodes UDPScanner::runScan(int portNum, bool isAdmin, string IPToScan){
    
    // UDP is typically more targeted scan, due to the time it takes
    
    // make a socket to scan ports
    int ourUDPSock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // if the socket was unsuccessful
    if(ourUDPSock == -1){
        cout << "Dgram error \n";
        return NoobCodes::socketCreationErrorDGRAM;
    }
    
    // if Admin status is present, you can use ICMP and raw sockets to assist with the scan
    if(isAdmin){
        // make a RAW ICMP socket to receive port status later
        int ourRawICMPSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        // confirm ICMP socket was successful
        if(ourRawICMPSock == -1){
            printf("Error: %s\n", strerror(errno));
            cout << "ICMP access error. UDP scan will be limited.\n";
            //return NoobCodes::socketCreationErrorICMP;
        }
    }
    else{
        cout << "As you are not root, you cannot access ICMP/raw sockets. Your UDP scan will be more limited.\n";
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
    
    string testString="test scan\nhello\n";
    
    // check port (sendto returns ssize_t and not int)
    ssize_t portCheck=sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
    
    // sleep while we wait for port response
    //usleep(this->ourSleepTimer);
    
    // return status of scan
    // TODO: make send and receive checks their own functions, to review errno and report back
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
            close(ourUDPSock);
        }
        return NoobCodes::portConnectionDenied;
    }
    //ICMP unreachable: port is closed. ICMP is rate limited: might not get this reply.
    //UDP reply: port open
    //no response: port is either open or filtered
    int length = sizeof(socketToScan);
    char ourBuffer[MAXRETURN];
    
    // set time out length of 3 seconds
    struct timeval timeout = {this->getSleepTimer(),0};
    // ensure our receive call doesn't block indefinitely
    setsockopt(ourUDPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    
    // after MAXRETURN, MSG_DONTWAIT can be used in place of the 0 to bypass any hangups. However, we'd want to wait for a designated time, just in case we can get a result.
    ssize_t receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
    
    // TODO: make send and receive checks their own functions, to review errno and report back
    // review packet receipt status and check for port open/closed/filtered indicators.
    if(receivedPacket==-1){
        
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << portNum << " closed. Initial read\n";
        }
        
        // if no ICMP access, try sending a set number of times to check for errors
        int retryTime=this->getRetries();
        for(int i = 0; i < retryTime; i++){
//            cout << "attempt " << i+1 << "\n";
//            cout << "Sending packet " << i+1 << "\n";
            
            // wait between sends to give packets a chance
            usleep(this->ourSleepTimer);
            
            // send another packet
            portCheck = sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
            
            // if send is denied, port is closed
            if(portCheck<0){
                cout << "Send error. Port likely closed.";
            }
            
            // otherwise, check to see if we have a packet response, indicating an open port
            else{
//                cout << "Receiving packet " << i+1 << "\n";
                // check for receive (remember to time out)
                receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
                
                // interpret receiving signal
                // if data is sent back, there is a response, and the port is likely open
                if(receivedPacket>0){
                    // if there is a response the port is likely open
                    cout << "Port " << portNum << " open - response received\n";
                    addPortList(portNum, this->openPorts);
                    close(ourUDPSock);
                    return NoobCodes::portConnectionSuccess;
                }
                
                // if 0 is sent back, the port is closed. Note that data is never guaranteed to be sent back for UDP.
                else if(receivedPacket==0){
                    cout << "Port " << portNum << " closed. recvfrom returned 0.\n";
                    addPortList(portNum, this->openPorts);
                    close(ourUDPSock);
                    return NoobCodes::portConnectionDenied;
                }
                else{
                    cout << "No packet received.\n";
                }
                
            }
        }
        
        // if sending/receiving multiple times yielded no results, check for any error flags on receive that would indicate port closure
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << portNum << " likely filtered (no data returned). Although you can increase your wait time, note that UDP is not obligated to respond.\n";
        }
        
        // close the socket
        close(ourUDPSock);
        return NoobCodes::portConnectionDenied;
    }
    else{
        // if there is a response, or no response, the port is likely open
        cout << "Port " << portNum << " open / filtered\n";
        addPortList(portNum, this->openPorts);
        close(ourUDPSock);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}

NoobCodes UDPScanner::runMultiScan(vector<unsigned> portNumbers, string IPToScan){
    // we'll come back to this once we work out a single scan.
    
    return NoobCodes::success;
}
