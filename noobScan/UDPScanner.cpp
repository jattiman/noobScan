/**/
/*
 UDPScanner.cpp of NoobScan, a novice portscanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/


#include "UDPScanner.h"
#define MAXRETURN 1024

/**/
/*
UDPScanner::UDPScanner() UDPScanner::UDPScanner()
NAME
        UDPScanner::UDPScanner - Class constructor
SYNOPSIS
        UDPScanner::UDPScanner();
DESCRIPTION
        Instantiats a UDPScanner with default parametrs.
RETURNS
        UDPScanner object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
UDPScanner::UDPScanner(){
    this->sleepTimer=getSleepTimer();
    this->variableScanTime=false;
    return;
}
/*UDPScanner();*/

/**/
/*
UDPScanner::UDPScanner() UDPScanner::UDPScanner()
NAME
         UDPScanner::UDPScanner - Class constructor
SYNOPSIS
        UDPScanner::UDPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan);
            newSleepTimer   --> The sleep timer value for assignmnt
            newTimeoutTimer --> The timeout timer value for assignment
            variableScan    --> The variable scan toggle (true/false) for assignment
DESCRIPTION
        This function instantiates the UDPScanner object with non-default values, and ensures that its sleep timer, timeout timer, and variable scan are all working accordingly.
RETURNS
        UDPScanner object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
UDPScanner::UDPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan){
    this->sleepTimer=newSleepTimer;
    this->timeoutTimer=newTimeoutTimer;
    this->variableScanTime=variableScan;
    return;
}
/*UDPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan);*/

/**/
/*
UDPScanner::runScan() UDPScanner::runScan()

NAME
        UDPScanner::runScan - runs a port scan on an individual port.
SYNOPSIS
        NoobCodes UDPScanner::runScan(int portNum, bool isRoot, string IPToScan);
            portNum     --> port number to be scanned
            isRoot      --> if user is root
            IPToScan    --> string holding IP to scan
DESCRIPTION
        The runScan function runs a UDP scan on an individual port. Function is separated from multiscan for educational purposes, to help show programmers reading this documentation how a UDP scan can work at its base level. Contrary to my initial opinion, the UDP Scan was harder to set up than the TCP Scan, due to the different privilege levels needed for the scanner to run more effectively.
        The scanner starts by making sockets for our UDP sending socket, as well as our receiving socket (which is in a sockaddr_in struct, to bind IP manually to the port. It also checks the user's root privilege, which will determine if this scan will use ICMP sockets to determine port status.
        The function then binds the socket/port of our target, creates a test string to send, and sends the test string to the socket as indicated. The timeout variable indicates how long the function will wait for a port to respond before moving onto another port. Of note here, is that the UDP style does not guarantee a traditional response from the socket.
        Without root access, the scanner will send/receive multiple packets, and check to see if a response (from receive) or closed port indicator (send fails) is sent back.
        With root access, instead of sending/receiving multiple packets, the scanner will wait according to the user's settings, then attempt to read any ICMP packet being sent back to the user. This packet header will be interpreted to determine the ICMP signals ('icmp_type's, specifically) sent back, which can help it determine if a port is reporting that it's closed, or is simply unresponsive.
        UDP is by definition unreliable, as you may be able to determine from the above information.
RETURNS
        NoobCode enum indicating the success or failure of the scan.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes UDPScanner::runScan(int portNum, bool isRoot, string IPToScan){
        
    // make a socket to scan ports
    int ourUDPSock = socket(AF_INET, SOCK_DGRAM, 0);
    
    // holder for recvfrom packets
    ssize_t receivedPacket;
    
    // if the socket was unsuccessful
    if(ourUDPSock == -1){
        // end early
        return NoobCodes::socketCreationErrorDGRAM;
    }
    
    // if root status isn't present, inform the user that their scan will not be as powerful
    if(!isRoot){

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
    
    // return status of scan
    if(!sendCheck(portCheck, portNum)){
        close(ourUDPSock);
        return NoobCodes::portSendDenied;
    }
    
    //UDP reply: port open. No response: port is either open or filtered
    int length = sizeof(socketToScan);
    char ourBuffer[MAXRETURN];
    
    // set time out length
    struct timeval timeout = {this->getTimeoutTimer(),0};
    
    // if root status is present, you can use ICMP and raw sockets to assist with the scan
    if(isRoot){
        
        // make a RAW ICMP socket to receive port status later. Note: ICMP sockets are SOCK_RAW, and explictly follow ICMP protocol (IPPROTO_ICMP)
        int receivingICMPSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

        // confirm ICMP socket was successful
        
        // if it wasn't successful, abandon ICMP and inform user we'll be doing an ICMPless UDP scan
        if(receivingICMPSock == -1){
            printf("Error: %s\n", strerror(errno));
            cout << "ICMP access error. UDP scan will be limited.\n";
        }
        
        // if ICMP creation was successful, test packet receipt through it
        else{
            
            // ensure our receive call doesn't block indefinitely
            setsockopt(receivingICMPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
            
            // receive from port
            receivedPacket = recvfrom(receivingICMPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
            
            // create IP structure, and feed the received buffer into it
            struct ip* ourIPHeader = (struct ip*) ourBuffer;
            
            // bitwise left shift the IP_HL portion of our packet to interpret length (whew)
            int ipLength = ourIPHeader->ip_hl << 2;
            
            // add the combined buffer and length into an ICMP structure
            struct icmp* ourICMP = (struct icmp * ) (ourBuffer + ipLength);
            
            // If the ICMP reports unreachable, then the port is closed. ICMP is rate limited: might not get this reply.
            // TODO: convert to function to review replies. This is sufficient for now, though.
            if((ourICMP->icmp_type == ICMP_UNREACH) && (ourICMP->icmp_code == ICMP_UNREACH_PORT)){
                cout << "ICMP port unreachable (closed).\n";
                cout << "\ttype: " << ourICMP->icmp_type << "\tcode: " << ourICMP->icmp_code << endl;
                addPortList(portNum, this->closedPorts);
                close(ourUDPSock);
                return NoobCodes::portConnectionDenied;
            }
            // we assume the port is open or filtered if
            else{
                cout << "Port " << portNum << "open / filtered\n";
                cout << "\ttype: " <<ourICMP->icmp_type << "\tcode: " << ourICMP->icmp_code << endl;
                addPortList(portNum, this->openPorts);
                close(ourUDPSock);
                return NoobCodes::portConnectionSuccess;
            }
        }
    }
    
    // ensure our receive call doesn't block indefinitely
    setsockopt(ourUDPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    
    // after MAXRETURN, MSG_DONTWAIT can be used in place of the 0 to bypass any hangups. However, we'd want to wait for a designated time, just in case we can get a result.
    receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
    
    // if you are not an admin, or if the ICMP status was not successful, send and receive additional packets through non-root to see if we can get a clue about the server
    // review packet receipt status and check for port open/closed/filtered indicators, as non-root
    if(receivedPacket==-1){
        
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << portNum << " waiting. Initial read\n";
        }
        
        // if no ICMP access, try sending a set number of times to check for errors
        int retryTime=this->getRetries();
        for(int i = 0; i < retryTime; i++){
            
            // wait between sends to give packets a chance
            usleep(this->getSleepTimer());
            
            // send another packet
            portCheck = sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
            
            // if send is denied, port is closed
            if(portCheck<0){
                cout << "Send error. Port likely closed.\n";
                addPortList(portNum, this->closedPorts);
                close(ourUDPSock);
                return NoobCodes::portConnectionDenied;
            }
            
            // otherwise, check to see if we have a packet response, indicating an open port
            else{
                
                // check for receive (remember to time out)
                receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
                
                // interpret receiving signal
                // if data is sent back, there is a response, and the port is likely open
                if(receivedPacket>0){
                    // if there is a response the port is likely open
                    cout << "Port " << portNum << " open - response received\n";
                    
                    // add it to open port lists
                    addPortList(portNum, this->openPorts);
                    
                    // close the port, and return connection success
                    close(ourUDPSock);
                    return NoobCodes::portConnectionSuccess;
                }
                
                // if 0 is sent back, the port is closed. Note that data is never guaranteed to be sent back for UDP. With ICMP, you may receive a more obvious closed port signal (typically a RST)
                else if(receivedPacket==0){
                    cout << "Port " << portNum << " closed. recvfrom returned 0.\n";
                    addPortList(portNum, this->closedPorts);
                    close(ourUDPSock);
                    return NoobCodes::portConnectionDenied;
                }
                
                // if no packet is received, the port is likely filtered (firewall settings).
                else{
                    // do nothing until the loop ends
                }
                
            }
        }
        
        // if sending/receiving multiple times yielded no results, check for any error flags on receive that would indicate port closure
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << portNum << " likely filtered (no data returned). Although you can increase your wait time, note that UDP is not obligated to respond.\n";
        }
        
        // close the socket
        close(ourUDPSock);
    }
    // if the report returns 0 bytes, peer has shut down purposefully, and port is closed
    else if(receivedPacket == 0){
        cout << "Port " << portNum << " shut down\n";
        addPortList(portNum, this->closedPorts);
        close(ourUDPSock);
        return NoobCodes::portConnectionDenied;
    }
    // otherwise, if there is a response, the port is likely open
    else{
        // if there is a response, or no response, the port is likely open
        cout << "Port " << portNum << " open / filtered\n";
        addPortList(portNum, this->openPorts);
        close(ourUDPSock);
        return NoobCodes::portConnectionSuccess;
    }
    
    return NoobCodes::success;
}
/*NoobCodes runScan(int portNum, bool isRoot, string IPToScan="127.0.0.1");*/

/**/
/*
UDPScanner::runMultiScan() UDPScanner::runMultiScan()

NAME
        UDPScanner::runMultiScan - runs a scan on multiple ports
SYNOPSIS
        NoobCodes UDPScanner::runMultiScan(vector<unsigned> portNumbers, bool isRoot, string IPToScan);
            portNumbers --> vector of all port numbers to be scanned
            isRoot      --> tells program if user is root
            IPToScan    --> the string holding the IP to be scannd
DESCRIPTION
        This function is very similar to the UDP single port scanner, but has been broken up to scan multiple ports in a single function. This is the heart of the UDPScanner object, as all normal user scans run through it (only the assisted scan option does not). The main difference here is now the function runs, as it uses a loop to iterate through the different ports. Reviewing the structure will help show you which order the socket should be created in, and how the IP would bind with the port in the socket.
        In future iterations of this project, I will create a scan handler class which will call the ICMP or non-ICMP versions of the UDP scan. This will shorten this function length greatly (as well as the runScan function), and allow me to show how a pure ICMP or non-ICMP UDP scan works. For now, it is sufficient.
RETURNS
        NoobCodes enum indicating the success or failure of the scan.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes UDPScanner::runMultiScan(vector<unsigned> portNumbers, bool isRoot, string IPToScan){
    
    // holder for recvfrom packets
    ssize_t receivedPacket;
    ssize_t receivedICMPPacket;
    
    // holder for receiving ICMP socket
    int receivingICMPSock;
    
    // if root status isn't present, inform the user that their scan will not be as powerful
    if(!isRoot){

        cout << "As you are not root, you cannot access ICMP/raw sockets. Your UDP scan will be more limited.\n";
    }

    // create socket structure to check port
    struct sockaddr_in socketToScan;
    socketToScan.sin_family = AF_INET;
    
    // convert IP address accordingly
    if(!inet_pton(AF_INET, IPToScan.c_str(), &socketToScan.sin_addr)){
        std::cout << "IP binding issue\n";
        return NoobCodes::IPBindingIssue;
    }
    
    // set time out length (this will not change)
    struct timeval timeout = {this->getTimeoutTimer(),0};
    
    string testString="test scan\nhello\n";
    
    for(auto const & nextPort: portNumbers){
                
        // make a socket to UDP scan ports
        int ourUDPSock = socket(AF_INET, SOCK_DGRAM, 0);
        
        // if the socket was unsuccessful
        if(ourUDPSock == -1){
            // end early. If there's something wrong with sockets on our end, there's probably a big issue
            return NoobCodes::socketCreationErrorDGRAM;
        }
        
        // assign the next port to our target socket
        socketToScan.sin_port = htons(nextPort);
    
        // if variable scanning is on, wait according to user time + random increment between scans
        if(getVariableScanStatus()){
            usleep(this->getSleepTimer() + generateNewSeed());
        }
        
        // otherwise, wait according to sleep timer
        else{
            usleep(this->getSleepTimer());
        }
        
        // check port (sendto returns ssize_t and not int)
        ssize_t portCheck=sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
        
        // return status of scan
        if(!sendCheck(portCheck, nextPort)){
            // if send returns error, mark as closed and continue to next port
            cout << "\tSend error - going to next port\n";
            addPortList(nextPort, this->closedPorts);
            close(ourUDPSock);
            continue;
        }
        
        
        int length = sizeof(socketToScan);
        char ourBuffer[MAXRETURN];
        
        // if root status is present, you can use ICMP and raw sockets to assist with the scan
        if(isRoot){
            
            // make a RAW ICMP socket to receive port status later. Note: ICMP sockets are SOCK_RAW, and explictly follow ICMP protocol (IPPROTO_ICMP)
            receivingICMPSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

            // confirm ICMP socket was successful
            // if it wasn't successful, abandon ICMP and inform user we'll be doing an ICMPless UDP scan
            if(receivingICMPSock == -1){
                printf("Error: %s\n", strerror(errno));
                cout << "\tICMP access error. UDP scan will be limited.\n";
                // jump to the next port - maybe we'll get lucky
                continue;
            }
            
            // if ICMP creation was successful, test packet receipt through it
            // ensure our receive call doesn't block indefinitely
            setsockopt(receivingICMPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
            
            // receive from port. Although we're storing the return value, we're more interested in the buffer return for ICMP purposes. We'll be interpreting this buffer to see what the ICMP status is, if any.
            receivedICMPPacket = recvfrom(receivingICMPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
            
            // create IP structure, and feed the received buffer into it
            struct ip* ourIPHeader = (struct ip*) ourBuffer;
            
            // bitwise left shift the IP_HL portion of our packet to interpret length (whew)
            int ipLength = ourIPHeader->ip_hl << 2;
            
            // add the combined buffer and length into an ICMP structure
            struct icmp* ourICMP = (struct icmp * ) (ourBuffer + ipLength);
            
            // If the ICMP reports unreachable, then the port is closed. If ICMP is rate limited, we might not get this reply.
            ICMPCheck(ourICMP, nextPort);
            close(ourUDPSock);
            memset(&ourBuffer[0], 0, sizeof(ourBuffer));
            continue;
            
        }
        
        // if you aren't root, or if ICMP failed, continue with receiving sockets the old way
        
        // ensure our receive call doesn't block indefinitely
        setsockopt(ourUDPSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        
        //Remember, if we get a UDP reply, the port is likely open. If we get no response: port is likely filtered.
        // When receiving, after MAXRETURN, MSG_DONTWAIT can be used in place of 0 to bypass any hangups. However, we'd want to wait for a designated time, just in case we can get a result, based on how UDP works
        receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
        
        // if you are not an admin, or if the ICMP status was not successful, send and receive additional packets through non-root to see if we can get a clue about the server
        // review packet receipt status and check for port open/closed/filtered indicators, as non-root
        if(receivedPacket==-1){
            
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                cout << "Port " << nextPort << " closed. Initial read\n";
            }
            
            // if no ICMP access, try sending a set number of times to check for errors
            int retryTime=this->getRetries();
            for(int i = 0; i < retryTime; i++){
                
                // wait between sends to give packets a chance
                usleep(this->getSleepTimer());
                
                // send another packet
                portCheck = sendto(ourUDPSock, testString.c_str(), testString.size()+1, 0, (sockaddr*)&socketToScan, sizeof(socketToScan));
                
                // if send is denied, port is closed
                if(portCheck<0){
                    cout << "Send error. Port " << nextPort << " likely closed.\n";
                    addPortList(nextPort, this->closedPorts);
                    close(ourUDPSock);
                    break;
                }
                
                // otherwise, check to see if we have a packet response, indicating an open port
                else{
                    
                    // check for receive (remember to time out)
                    receivedPacket = recvfrom(ourUDPSock, ourBuffer, MAXRETURN, 0, (struct sockaddr *) &socketToScan, (socklen_t *) &length);
                    
                    // interpret receiving signal
                    // if data is sent back, there is a response, and the port is likely open
                    if(receivedPacket>0){
                        // if there is a response the port is likely open
                        cout << "Port " << nextPort << " open - response received\n";
                        
                        // add it to open port lists
                        addPortList(nextPort, this->openPorts);
                        
                        // close the port, and return connection success
                        close(ourUDPSock);
                        break;
                    }
                    
                    // if 0 is sent back, the port is closed. Note that data is never guaranteed to be sent back for UDP. With ICMP, you may receive a more obvious closed port signal (typically a RST)
                    else if(receivedPacket==0){
                        cout << "Port " << nextPort << " closed. recvfrom returned 0.\n";
                        addPortList(nextPort, this->closedPorts);
                        close(ourUDPSock);
                        break;
                    }
                    
                    // if no packet is received, the port is likely filtered (firewall settings).
                    else{
                        // do nothing until the loop ends
                    }
                }
            }
            
            // if sending/receiving multiple times yielded no results, check for any error flags on receive that would indicate port closure
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                cout << "Port " << nextPort << " likely filtered (no data returned). Although you can increase your wait time, note that UDP is not obligated to respond.\n";
            }
            
            // close the socket
            close(ourUDPSock);
            continue;
        }
        // if the report returns 0 bytes, peer has shut down purposefully, and port is closed
        else if(receivedPacket == 0){
            cout << "Port " << nextPort << " shut down\n";
            addPortList(nextPort, this->closedPorts);
            close(ourUDPSock);
            continue;
        }
        // otherwise, if there is a response, the port is likely open
        else{
            // if there is a response, or no response, the port is likely open
            cout << "Port " << nextPort << " open / filtered\n";
            addPortList(nextPort, this->openPorts);
            close(ourUDPSock);
            continue;
        }
    
    }
    return NoobCodes::success;
}
/*NoobCodes runMultiScan(vector<unsigned> portNumbers, bool isRoot, string IPToScan="127.0.0.1");*/


/**/
/*
UDPScanner::sendCheck() UDPScanner::sendCheck()

NAME
        UDPScanner::sendCheck - checks the return value of send for each socket
SYNOPSIS
        bool UDPScanner::sendCheck(ssize_t ourSocket, int ourPort);
            ourSocket --> socket send value being reviewed
            ourPort   --> port being reviewed
DESCRIPTION
        This function intakes a sendto() function's resulting value, and determines if it was successful. If successful, it will return true. If not successful, it will determine and output the error to the screen, before returning false, so that the scanning functions can operate on them accordingly.
RETURNS
        A bool variable indicating if the send function was successful or not.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// checks the sending socket to see if there are issues
bool UDPScanner::sendCheck(ssize_t ourSocket, int ourPort){
    if(ourSocket<0){
        cout << "oursocket < 0 \n";
        // if there is a connection error, the port is likely closed
        // review error status, and report why it's closed
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            cout << "Port " << ourPort << " closed: nonblocking port blocked by request\n";
        }
        else if(errno == EBADF){
            cout << "Port " << ourPort << " file descriptor issue\n";
        }
        else if(errno == EFAULT){
            cout << "Port " << ourPort << " rejected userspace address\n";
        }
        else if(errno == EINVAL){
            cout << "Port " << ourPort << " received invalid argument\n";
        }
        else if(errno == EMSGSIZE){
            cout << "Port " << ourPort << " issue: message size error \n";
        }
        else if(errno == ENOBUFS){
            cout << "Port " << ourPort << " issue: output queue full\n";
        }
        else if(errno == EOPNOTSUPP){
            cout << "Port " << ourPort << " had issue with flags\n";
        }
        else{
            cout << "Port " << ourPort << " likely closed\n";
        }
        //addPortList(ourPort, this->closedPorts);
        return false;
    }
    return true;
}
/*bool sendCheck(ssize_t ourSocket, int ourPort);*/

/**/
/*
UDPScanner::ICMPCheck() UDPScanner::ICMPCheck()

NAME
        UDPScanner::ICMPCheck - check the ICMP status to help determine port open/fitered/closed status
SYNOPSIS
        bool UDPScanner::ICMPCheck(struct icmp* checkICMP, unsigned int nextPort);
            checkICMP   --> the ICMP structure of the scan
            nextPort    --> the port number currently being worked on
DESCRIPTION
        This function will review the ICMP structure sent to it to determine the value of icmp_type. This will allow the function to determine if the port is open or closed. It will report back the status to the user. Note: ICMP may be rate limited by some companies, which would mean that the structure would not receive a response. If this happens, the program will unfortunately assume that the port is filtered/open (which it could be). The professional port scanner, nmap, just recently published an update meant to get around ICMP rate limiting. Very interesting, huh?
RETURNS
        Bool indicating if the port is possibly open or closed.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// check ICMP status to determine port outcome
bool UDPScanner::ICMPCheck(struct icmp* checkICMP, unsigned int nextPort){
    bool possibleOpen = false;
    
    if((checkICMP->icmp_type == ICMP_UNREACH) && (checkICMP->icmp_code == ICMP_UNREACH_PORT)){
        cout << "\tICMP port " << nextPort << " unreachable (closed).\n";
        addPortList(nextPort, this->closedPorts);
    }
    
    // we assume the port is open or filtered, otherwise
    else{
        cout << "\tPort " << nextPort << " open / filtered\n";
        addPortList(nextPort, this->openPorts);
        possibleOpen=true;
    }
    
    return possibleOpen;
}
/*bool ICMPCheck(struct icmp* checkICMP, unsigned int nextPort);*/

/**/
/*
UDPScanner::getSleepTimer() UDPScanner::getSleepTimer()

NAME
        UDPScanner::getSleepTimer - gets sleep timer
SYNOPSIS
        unsigned int UDPScanner::getSleepTimer();
DESCRIPTION
        This is a getter function to return the sleep timer.
RETURNS
        Unsigned int representing the sleep timer variable of the class.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// get sleep time
unsigned int UDPScanner::getSleepTimer(){
    return this->sleepTimer;
}
/*unsigned int getSleepTimer();*/

/**/
/*
UDPScanner::getTimeoutTimer() UDPScanner::getTimeoutTimer()

NAME
        UDPScanner::getTimeoutTimer - gets timeout timer
SYNOPSIS
        unsigned int UDPScanner::getTimeoutTimer();
DESCRIPTION
        This is a getter function to return the timeout timer.
RETURNS
        Unsigned int representing the timeout timer variable of the class.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// get timeout time
unsigned int UDPScanner::getTimeoutTimer(){
    return this->timeoutTimer;
}
/*unsigned int getTimeoutTimer();*/

/**/
/*
UDPScanner::getVariableScanStatus() UDPScanner::getVariableScanStatus()

NAME
        UDPScanner::getVariableScanStatus - returns if variable scanning is on
SYNOPSIS
        bool UDPScanner::getVariableScanStatus(){
DESCRIPTION
        This is a getter function to return the status of variable scanning.
RETURNS
        A bool value representing if variable scanning is on or off.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool UDPScanner::getVariableScanStatus(){
    return this->variableScanTime;
}
/*bool getVariableScanStatus();*/
