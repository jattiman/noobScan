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

class UDPScanner{
    
public:
    UDPScanner();
    
    // run scan - default value is own IP
    NoobCodes runScan(int portNum, std::string IPToScan="127.0.0.1");
    
private:
    
};

#endif /* UDPScanner_h */