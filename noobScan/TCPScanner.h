//
//  TCPScanner.h
//  noobScan
//
//  Created by John Atti on 4/22/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#ifndef TCPScanner_h
#define TCPScanner_h

#include "NoobCodes.h"
#include "ScanAddress.h"

//delete these 2 later if this doesn't work
#include <vector>
using namespace std;


class TCPScanner : public ScanAddress{

public:
    TCPScanner();

    // add open port to list
    void addOpenPorts(int newOpenPort);
    
    // get open port list
    vector<int> getOpenPorts();
    
    // print open ports
    void printOpenPorts();
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, std::string IPToScan="127.0.0.1");
    
    // scan multiple ports at once
    NoobCodes runMultiScan(vector<int> portNumbers, std::string IPToScan="127.0.0.1");
    
    
    
private:
    vector<int> openPorts;
    
    // not sure if I'll use this
    vector<int> closedPorts;
    
};

#endif /* TCPScanner_h */
