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
#include <vector>

using namespace std;

class ScanAddress{
    
public:
    // confirm the scan type being requested
    char checkScanType(std::string scanCommand);
    
    // initiate scan, report results and output to screen
    string initiateScan(char ourScanType);
    
    // resolve host name
    struct hostent * returnHostIP(std::string targetHost);
    
    // list ports being scanned
    void getPortList();
    
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
    
    // vector of ports to scan
    vector<int> portsToScan;
    
};

#endif /* ScanAddress_h */
