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
#include <string>

class Recorder {
    
public:
    // class initialization
    Recorder();
    
    // records user request
    void recordRequest(std::string userRequest);
    
    // purges history fully
    void clearHistory();
    
    // shows all user history
    void showHistory();
    
    // shows all user commands
    void showAllRequests();
    
    // shows only successfull user commands
    void showSuccessfulRequests();
    
    // copy commands to file (class controls file)
    void copyAllHistory();
    
private:
    std::vector<std::string> ourRecord;
};

#endif /* Recorder_h */
