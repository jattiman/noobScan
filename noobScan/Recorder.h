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
#include <iomanip>

// file manipulation
#include <fstream>
#include <sstream>

// time
#include <ctime>

#include "NoobCodes.h"

using namespace std;

class Recorder {
    
public:
    // class initialization
    Recorder();
    
    // records requests by type
//    void categorizeRequest(string userRequest, char category='x');
    
    void categorizeOutcome(string userRequest, char outcome, char category='x');
    
    // records user request
    void tallyRequest(char category);
    
    // purges history fully
    void clearHistory();
    
    // shows all user history
    void showHistory(char historyType='a');
    
    // shows history by type
    string writeHistoryByType(char historyType='a');
    
    // copy commands to file (class controls file)
    void moveHistoryToFile();
    
    // set the recorder on/off status
    void setRecorderStatus(bool recorderStatus);
    
    // returns if the recorder is on or off
    bool getRecorderStatus();
    
private:
    vector<string> ourRecord;
    unordered_map<char, vector<string>> commandMap;
    unordered_map<char, vector<std::pair <string, char>>> outcomeMap;
    unordered_map<string, unsigned int> tallyMap;
    bool isRecorderOn;
};

#endif /* Recorder_h */
