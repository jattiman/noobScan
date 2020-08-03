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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//for IP address
#include <netinet/ip.h>

// for ICMP usage - must include after ip.h to avoid conflicts
#include <netinet/ip_icmp.h>

// to pull MAC address
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#include <errno.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>

#include "NoobCodes.h"

using namespace std;

class ScanAddress{
    
public:
    
    ScanAddress();
    
    // confirm user input is valid
    int getValidInput(int minNum=0, int maxNum=65535);
    
    // confirm the scan type being requested
    char checkScanType(string scanCommand);
    
    // initiate scan, report results and output to screen
    string initiateScan(char ourScanType);
    
    // resolve target IP from URL
    string getTargetIP(string targetHost);
    
    // print host name in human readable format
    string getHostIP(string ifaNamePreference = "en0");
    
    // add to port list
    void addPortList(int newPortNumber, vector<int> portVector);
    
    // list ports being scanned
    void getPortList();
    
    // check port number validity
    NoobCodes portValidityCheck(int portNumToCheck);//0 and 65535
    
    // get sleep time
    unsigned int getSleepTimer();
    
    // get host MAC address
    string getHostMac(string ifaNamePreference = "en0");
    
    // get port retry number
    int getRetries();
        
    // get the user interface
    string getInterface();
    
    // variable delay status
    bool getVariableDelayStatus();
    
    // set sleep time
    void setSleepTimer(unsigned int newSleepTime);
    
    // set the retry number
    void setRetries(int newRetryAmount);
    
    // set the user interface
    void setInterface(string ifType);
    
    // set variable delay
    void setVariableDelayStatus(bool variableDelayStatus);
    
    // training wheels scan to prompt user step by step
    NoobCodes assistedScan();
    
    // print debug statement
    void debug();
    
    
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
    
    // variable delay between scans
    bool variableDelay;
    
    // vector of ports to scan
    vector<int> portsToScan;
    
    // sleep timer
    unsigned int sleepTimer;
    
    // interface the use
    string ourInterface;
    
    // our mac address
    char ourMac[32]={0};
    
    // number of times to retry certain ports if there's a connection error
    int retries;
    
};

#endif /* ScanAddress_h */
