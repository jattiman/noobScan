/**/
/*
   Scanaddress.h of noobScan, a novice port scanner
   Scanaddress.h
 Controls the settings of the scanner
*/
/**/

#ifndef ScanAddress_h
#define ScanAddress_h

//#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>

class ScanAddress{
    
public:
    // confirm the scan type being requested
    char checkScanType(std::string scanCommand);
    
    // initiate scan, report results and output to screen
    std::string initiateScan(char ourScanType);
    
    // resolve host name
    struct hostent * returnHostIP(std::string targetHost);
    
    // training wheels scan to prompt user step by step
    
    
    
private:
    // array of ports
    unsigned short *portList;
    
    // code for scan type (tcp/udp)
    char scanType;
    
    // time out time
    int timeOut;
    
    // save to file
    bool saveToFile;
    
    // output to screen
    bool screenOutput;
    
    
    
};

#endif /* ScanAddress_h */
