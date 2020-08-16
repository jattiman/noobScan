/**/
/*
 SYNScanner.h of NoobScan, a novice portscanner
 SYNScanner.h does nothing - it is not yet implemented, and will be worked on as I learn more about network programming, post-grad. I will not be removing this from project submission, as I intend on completing it after graduation, and do not want to remove it from the repo.
 Author: John Atti
 Not finished
*/
/**/

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
