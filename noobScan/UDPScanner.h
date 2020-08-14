//
//  UDPScanner.hpp
//  noobScan
//
//  Created by John Atti on 4/26/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

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
