/**/
/*
   HelpModule.h of noobScan, a novice port scanner
   HelpModule.h is responsible for mantaining (reading from / writing to) the dictionary that informs users about the commands, ports, and functions of the program. It also updates a map with dictionary terms for easy user viewing, and to minimize file access time.
 Author: John Atti
 Finished 8/16/2020
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
    
    // confirms user entered valid int
    int getValidInput(int minNum=0, int maxNum=65535);
    
    // output additional feedback
    void outputFeedback(string feedback);

    // displays opening prompt for the user to select info for
    void displayDirections();
    
    // tells the user everything they can say
    void displayOptions();
    
    // returns help results based on the user request
    void returnInfo(string userString);
    void returnInfo(int userNum);
    
    // make string lowercase
    void convertToLowerCase(string & ourString);
    
    // fill in the dictionary from file
    void populateDirectory();
    
    // print the entire dictionary
    void printFullDirectory();
    
    // prompt user to add entry
    void promptToAdd();
    
    // add entry to dictionary file
    void addToDictionary();
    
    // add individual entry to help directory
    void addToHelpDirectory(string word, string definition);
    
    // prompt user for new dictionary entry
    string getNewEntry();
    
    // give a warning to the user if they're asking too much of the help function
    void helpWarning();
    
    // set system feedback status
    void setSystemFeedback(bool isOn);
    
    // get system feedback status
    bool getSystemFeedback();

    
private:
    std::map<string,string> helpDirectory;
    
    bool systemFeedback = true;
};

#endif /* HelpModule_h */
