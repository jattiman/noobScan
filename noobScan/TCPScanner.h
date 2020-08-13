//
//  TCPScanner.h
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#ifndef TCPScanner_h
#define TCPScanner_h

//#include "NoobCodes.h"
#include "ScanAddress.h"
#include <vector>
using namespace std;


class TCPScanner : public ScanAddress{

public:
    TCPScanner();
    
    TCPScanner(unsigned int sleep);

    // add open port to list
    void addOpenPorts(int newOpenPort);
    
    // add closed port to list
    void addClosedPorts(int newClosedPort);
    
    // get open port list
    vector<int> getOpenPorts();
    
    // print open ports
    void printOpenPorts();
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, string IPToScan);
    
    // scan multiple ports at once
    NoobCodes runMultiScan(vector<unsigned> portNumbers, string IPToScan);
    
    // get sleep time
    unsigned int getSleepTimer();
       
    // get timeout time
    unsigned int getTimeoutTimer();
    
    
private:
    vector<int> openPorts;
    vector<int> closedPorts;
    unsigned int sleepTimer;
    unsigned int timeoutTimer;
    
};

#endif /* TCPScanner_h */
