/**/
/*
   Recorder.h of noobScan, a novice port scanner
   Recorder.h
*/
/**/

#ifndef Recorder_h
#define Recorder_h

#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>


class Recorder {
    
public:
    // class initialization
    Recorder();
    
    // records requests by type
    void categorizeRequest(std::string userRequest, char category);
    
    // records user request
    void recordRequest(std::string userRequest);
    
    // purges history fully
    void clearHistory();
    
    // shows all user history
    void showHistory();
    
    // shows history by type
    void showTypeHistory(char category);
    
    // shows all user commands
    void showAllRequests();
    
    // shows only successfull user commands
    void showSuccessfulRequests();
    
    // copy commands to file (class controls file)
    void copyAllHistory();
    
private:
    std::vector<std::string> ourRecord;
    std::unordered_map<char, std::vector<std::string>> commandMap;
    
};

#endif /* Recorder_h */
