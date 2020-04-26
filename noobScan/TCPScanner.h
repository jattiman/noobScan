//
//  TCPScanner.h
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#ifndef TCPScanner_h
#define TCPScanner_h

#include <iostream>
#include <vector>
#include "NoobCodes.h"
#include "ScanAddress.h"

class TCPScanner{

public:
    TCPScanner();

    // add open port to list
    void addOpenPorts(int newOpenPort);
    
    // get open port list
    std::vector<int> getOpenPorts();
    
    // print open ports
    void printOpenPorts();
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, std::string IPToScan="127.0.0.1");
    
    
    
private:
    std::vector<int> openPorts;
    
    // not sure if I'll use this
    std::vector<int> closedPorts;
    
};

#endif /* TCPScanner_h */
