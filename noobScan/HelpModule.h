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
#include <map>
#include <iostream>

class HelpModule{
    
public:
    HelpModule();
    
    // displays opening prompt for the user to select info for
    void displayDirections();
    
    // tells the user everything they can say
    void displayOptions();
    
    // returns help results based on the user request
    void returnInfo(std::string userString);

    
private:
    std::map<std::string,std::string> helpDirectory;
};

#endif /* HelpModule_h */
