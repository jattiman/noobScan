/**/
/*
   HelpModule.h of noobScan, a novice port scanner
   HelpModule.h
*/
/**/

#ifndef HelpModule_h
#define HelpModule_h

#include <stdio.h>
#include <string>
#include <iostream>

class HelpModule{
    
public:
    HelpModule();
    
    // displays opening prompt for the user to select info for
    void displayDirections();
    
    // returns help results based on the user request
    void returnInfo(std::string userString);

    
private:
    
};

#endif /* HelpModule_h */
