/**/
/*
 TCPScanner.cpp of NoobScan, a novice portscanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#include "TCPScanner.h"

/**/
/*
TCPScanner::TCPScanner() TCPScanner::TCPScanner()

NAME
        TCPScanner::TCPScanner - class constructor
SYNOPSIS
        TCPScanner::TCPScanner()
DESCRIPTION
        This is the class constructor for TCPScanner.
RETURNS
        TCPScanner object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
TCPScanner::TCPScanner(){
    this->openPorts.reserve(1000);
    this->variableScanTime = false;
}
/*TCPScanner();*/

/**/
/*
TCPScanner::TCPScanner() TCPScanner::TCPScanner()

NAME
        TCPScanner::TCPScanner - class constructor
SYNOPSIS
        TCPScanner::TCPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan);
            newSleepTimer   --> sleep timer variable
            newTimeoutTimer --> timeout timer variable
            variableScan    --> bool determining if variable scan is on
DESCRIPTION
        This will instantiate a TCPScanner object with sleep, timeout, and variable scanning preferences recorded.
RETURNS
        TCPScanner object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
TCPScanner::TCPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan){
    this->openPorts.reserve(1000);
    this->sleepTimer=newSleepTimer;
    this->timeoutTimer=newTimeoutTimer;
    this->variableScanTime=variableScan;
}
/*TCPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan);*/

/**/
/*
TCPScanner::addOpenPorts() TCPScanner::addOpenPorts()

NAME
        TCPScanner::addOpenPorts - adds port to open ports list
SYNOPSIS
        void TCPScanner::addOpenPorts(int newOpenPort);
            newOpenPort --> int holding new port number to add
DESCRIPTION
        This function takes in a new port number, and adds it to the open port list.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void TCPScanner::addOpenPorts(int newOpenPort){
    this->openPorts.emplace_back(newOpenPort);
    return;
}
/*void addOpenPorts(int newOpenPort);*/

/**/
/*
TCPScanner::addClosedPorts() TCPScanner::addClosedPorts()

NAME
        TCPScanner::addClosedPorts - adds to closed ports list
SYNOPSIS
        void TCPScanner::addClosedPorts(int newClosedPort);
            newClosedPort --> int to add to closed port list
DESCRIPTION
        This function adds a port to the closed ports list vector
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void TCPScanner::addClosedPorts(int newClosedPort){
    this->closedPorts.emplace_back(newClosedPort);
    return;
}
/*void addClosedPorts(int newClosedPort);*/

/**/
/*
TCPScanner::getOpenPorts() TCPScanner::getOpenPorts()

NAME
        TCPScanner::getOpenPorts - return vector of open ports
SYNOPSIS
        vector<int> TCPScanner::getOpenPorts();
DESCRIPTION
        This function returns a vector of open ports
RETURNS
        A vector<int> representing recorded open ports is returned.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
vector<int> TCPScanner::getOpenPorts(){
    return this->openPorts;
}
/*vector<int> getOpenPorts();*/

/**/
/*
TCPScanner::printOpenPorts() TCPScanner::printOpenPorts()

NAME
        TCPScanner::printOpenPorts - prints all open ports
SYNOPSIS
        void TCPScanner::printOpenPorts();
DESCRIPTION
        Pending open ports have been found, this prints all of the open ports to the screen.
RETURNS
        Void - no return values.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void printOpenPorts();*/

/**/
/*
TCPScanner::runScan() TCPScanner::runScan()

NAME
        TCPScanner::runScan - runs a TCP scan on a single port
SYNOPSIS
        NoobCodes TCPScanner::runScan(int portNum, string IPToScan);
            portNum     --> number of the port to be scanned
            IPToScan    --> IP address to be scanned
DESCRIPTION
        This function will scan a single port of a given IP address, output whether or not the port is open, and return the result.
        A socket must be created to send the packets, as well as one that represents the receiving socket (with the port we're sending it to).
        Once the sockets are created, we attempt to connect to the port, and review how this connection went.
        As this is a single port scan, we do not worry about time outs. The single port scan is being left into this program for educational purposes, as it shows the more straightforward steps to making a single scan. The multiscan (coming up next) is a little bit fancier.
        Structures and functions of note that are used, here: sockets (sockaddr_in) to store socket info, and inet_pton to bind with IP and port.
RETURNS
        NoobCodes enum stating if the scan was successful or not.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*NoobCodes runScan(int portNum, string IPToScan);*/

/**/
/*
TCPScanner::runMultiScan() TCPScanner::runMultiScan()

NAME
        TCPScanner::runMultiScan - runs TCP scan on multiple ports. Centerpiece of this class.
SYNOPSIS
        NoobCodes TCPScanner::runMultiScan(vector<unsigned> portNumbers, string IPToScan);
            portNumbers     --> vector holding port numbers to be scanned
            IPToScan        --> string holding Ip to be scanned
DESCRIPTION
        This function scans a group of ports to determine if they are open and closed, like runScan.
        Unlike runScan, it must account for timeouts and lining up multiple ports.
        To manage multiple ports, we use an auto-iterator loop that goes through the port vector given to the function.
        To manage timeouts, we utilize the select call, as well as fd_set type functions to organize the ports and cancel review of them if they don't get back to us in time. The timeout used in select is thanks to the timeval struct, which we use to indicate seconds and microseconds for the timer. With the select call, we also use getsockopt, which will return the status of the socket once it's ready to be acted on select (including if the timer is up).
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
        
        // clear the file descriptor (helpful since this is in a loop)
        FD_ZERO(&ourFD);
        
        // add our TCP socket into the set we'll be reviewing (a set of one, in this case)
        FD_SET(ourTCPSock, &ourFD);
        
        // determine if socket is open, based on select, as the above connect call will be "passed" even if it's "hanging" until our select timeout passes, or a response is received. NOTE: some sites (like mine) will still ignore an attempt to connect as a security feature, and the below code will not result in output, even though it passes. This program is not made to bypass this, and will play nice with the response style of the IP.
        if(select(ourTCPSock+1, NULL, &ourFD, NULL, &timeout) == 1){
            
            // int which will point to the returned buffer size in our TCP socket
            int errorCheck;
            socklen_t errorLen = sizeof(errorCheck);
            
            // check the result of our select call (which will move forward if the timeout hits, or if there's a connection response)
            getsockopt(ourTCPSock, SOL_SOCKET, SO_ERROR, &errorCheck, &errorLen);
            
            // If no error is present, confirm the port is open
            if(errorCheck==0){
                cout << "\tPort " << nextPort << " open\n";
                addOpenPorts(nextPort);
            }
            
            // If there's been an error, review the connectCheck response and act accordingly
            else{
                connectCheck(checkConnect, nextPort);
            }
        }
        
        // close the socket, and step to the next one if there are more left
        close(ourTCPSock);
    }
    
    return NoobCodes::success;
}
/*NoobCodes runMultiScan(vector<unsigned> portNumbers, string IPToScan);*/

/**/
/*
TCPScanner::connectCheck() TCPScanner::connectCheck()

NAME
        TCPScanner::connectCheck - review if connection was successful, and respond accordingly
SYNOPSIS
        void TCPScanner::connectCheck(int checkNum, unsigned int nextPort);
            checkNum --> this is the status of the connect call from the TCP socket
            nextPort --> this is the port number the socket is currently working on
DESCRIPTION
        This function reviews the connection outcome, and outputs to the user if it was successful or not. If the connection was successful, we add it to open ports. If the connection was not successful, we output why by reading the error response (timeout, busy, refused connection, etc).
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void TCPScanner::connectCheck(int checkNum, unsigned int nextPort){
    
    // if connection denied, return that info
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
/*void connectCheck(int checkNum, unsigned int nextPort);*/

/**/
/*
TCPScanner::getSleepTimer() TCPScanner::getSleepTimer()

NAME
        TCPScanner::getSleepTimer - retrieves sleep timer
SYNOPSIS
        unsigned int TCPScanner::getSleepTimer();
DESCRIPTION
        This function retrieves the member variable sleepTimer, which represents the sleep timer.
RETURNS
        Unsigned int representing sleep timer.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
unsigned int TCPScanner::getSleepTimer(){
    return this->sleepTimer;
}
/*unsigned int getSleepTimer();*/

/**/
/*
TCPScanner::getTimeoutTimer() TCPScanner::getTimeoutTimer()

NAME
        TCPScanner::getTimeoutTimer - get timeout timer
SYNOPSIS
        unsigned int TCPScanner::getTimeoutTimer();
DESCRIPTION
        This function retrieves the timeout timer variable.
RETURNS
        Unsigned int reflecting the timeout time.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
unsigned int TCPScanner::getTimeoutTimer(){
    return this->timeoutTimer;
}
/*unsigned int getTimeoutTimer();*/

/**/
/*
TCPScanner::getVariableScanStatus() TCPScanner::getVariableScanStatus()

NAME
        TCPScanner::getVariableScanStatus - retrieve whether or not variable scanning is on
SYNOPSIS
        bool TCPScanner::getVariableScanStatus();
DESCRIPTION
        This function retrieves whether or not the variable scan time option is on.
RETURNS
        A bool variable representing the variable scan time status (on/off as true/false).
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool TCPScanner::getVariableScanStatus(){
    return this->variableScanTime;
}
/*bool getVariableScanStatus();*/

