/**/
/*
   Recorder.cpp of noobScan, a novice port scanner
   Recorder.cpp records all of the users commands for educational purposes
*/
/**/

#include "Recorder.h"
#include <iostream>

// class initialization
Recorder::Recorder(){}

// records all user requests
void Recorder::recordRequest(std::string userRequest){
    ourRecord.push_back(userRequest);
    return;
}

// purges history fully
void Recorder::clearHistory(){
    ourRecord.clear();
    return;
}

// shows all user history
void Recorder::showHistory(){
    for(auto const & recordEntry: ourRecord){
        std::cout << recordEntry << std::endl;
    }
}

// shows all user commands
void Recorder::showAllRequests(){
    
}

// shows only successfull user commands
void Recorder::showSuccessfulRequests(){
    
}

// copy commands to file (class controls file)
void Recorder::copyAllHistory(){
    
}
