/**/
/*
   Scanaddress.h of noobScan, a novice port scanner
   Scanaddress.h
*/
/**/

#ifndef ScanAddress_h
#define ScanAddress_h

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

class ScanAddress{
    
private:
    // confirm the scan type being requested
    char checkScanType(std::string scanCommand);
    
    // initiate scan, report results and output to screen
    std::string initiateScan(char ourScanType);
    
public:
    // array of ports
    unsigned short *portList;
    char scanType;
    
    
    
};

#endif /* ScanAddress_h */
