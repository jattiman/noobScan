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

class UDPScanner : public ScanAddress{
    
public:
    UDPScanner();
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, bool isAdmin, string IPToScan="127.0.0.1");
    
    NoobCodes runMultiScan(vector<unsigned> portNumbers, string IPToScan="127.0.0.1");
    
    bool sendCheck(ssize_t ourSocket, int ourPort);
    
    
private:
    vector<int> openPorts;
    vector<int> closedPorts;
    struct timeval ourWaitTimer;
    unsigned int ourSleepTimer;
};

#endif /* UDPScanner_h */
