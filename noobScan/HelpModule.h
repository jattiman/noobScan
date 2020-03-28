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

class HelpModule{
    
private:
    HelpModule();
    
    // displays opening prompt for the user to select info for
    void displayOpeningText();
    
    // returns help results based on the user request
    void returnInfo(std::string userString);
    
    
    
    
public:
    
};

#endif /* HelpModule_h */
