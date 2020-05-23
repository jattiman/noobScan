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

// file manipulation
#include <iostream>
#include <fstream>
using namespace std;

class HelpModule{
    
public:
    HelpModule();
    
    // displays opening prompt for the user to select info for
    void displayDirections();
    
    // tells the user everything they can say
    void displayOptions();
    
    // returns help results based on the user request
    void returnInfo(string userString);
    void returnInfo(int userNum);
    
    // fill in the dictionary from file
    void populateDirectory();
    
    // print the entire dictionary
    void printFullDirectory();
    
    // prompt user to add entry
    void promptToAdd();
    
    // add entry to dictionary
    void addToDictionary();
    
    string getNewEntry();

    
private:
    std::map<string,string> helpDirectory;
};

#endif /* HelpModule_h */
