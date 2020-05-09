//
//  SYNScanner.hpp
//  noobScan
//
//  Created by John Atti on 5/9/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#ifndef SYNScanner_h
#define SYNScanner_h

#include "ScanAddress.h"
#include "NoobCodes.h"

class SYNScanner : public ScanAddress{

public:
    SYNScanner();
    
private:
    vector<int> openPorts;
    vector<int> closedPorts;
    
};

#endif /* SYNScanner_h */
