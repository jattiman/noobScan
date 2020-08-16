/**/
/*
   Recorder.h of noobScan, a novice port scanner
   Recorder.h records all user input when turned on, and categorizes it based on type (scan, help, etc) and outcome (if it succeeded perfectly, or failed even partially). Note that failure means that the command wasn't written correctly, or resulted in an error related to user input. The Recorder also manages output to a file summarizing the user commands and statistics, which the user can use to retrace their steps and see how they did.
 Author: John Atti
 Finished 8/16/2020
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
    
    // with recorder status
    Recorder(bool isOn);
    
    void categorizeOutcome(string userRequest, char outcome, char category='x');
    
    // tallies the amount of times a user has run a command
    void tallyUpdate(char category);
    
    // retrieve the count of tallies based on type
    string tallyRetrieval(char category);
    
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
    unordered_map<char, unsigned int> tallyMap;
    bool isRecorderOn;
};

#endif /* Recorder_h */
