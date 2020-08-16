/**/
/*
 UDPScanner.h of NoobScan, a novice portscanner
 UDPScanner.h is in charge of the functions pertaining to a UDP Scanner. It scans ports based on input, and outputs to user the open and closed ports encountered. Of note, is that UDP Scanner will function differently based on root privileges of the user. When user is root, UDPScanner will utilize ICMP packets to help determine if a port is open or closed after a timeout period has been reached. With a normal UDP scan, this is not possible (you would need to send various packets and check to see if anything was received a few times. Even then, the lack of response with the port would not guarantee accuracy). Without root, a user would not be able to utilize ICMP packets. Ona personal note: when I decided to go with UDP scanning, I thought it would be the easier of the 2 scans to implement well. This was not the case, due to the unreliable nature of UDP compared to TCP.
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#ifndef UDPScanner_h
#define UDPScanner_h

#include "NoobCodes.h"
#include "ScanAddress.h"
#include "HelpModule.h"

class UDPScanner : public ScanAddress{
    
public:
    UDPScanner();
    
    UDPScanner(unsigned int newSleepTimer, unsigned int newTimeoutTimer, bool variableScan);
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, bool isRoot, string IPToScan="127.0.0.1");
    
    NoobCodes runMultiScan(vector<unsigned> portNumbers, bool isRoot, string IPToScan="127.0.0.1");
    
    bool sendCheck(ssize_t ourSocket, int ourPort);
    
    bool receiveCheck();
    
    // check ICMP status to determine port outcome
    bool ICMPCheck(struct icmp* checkICMP, unsigned int nextPort);
    
    // get sleep time
    unsigned int getSleepTimer();
    
    // get timeout time
    unsigned int getTimeoutTimer();
    
    bool getVariableScanStatus();
    
private:
    vector<int> openPorts;
    vector<int> closedPorts;
    struct timeval ourWaitTimer;
    unsigned int sleepTimer;
    unsigned int timeoutTimer;
    bool variableScanTime;
};

#endif /* UDPScanner_h */
