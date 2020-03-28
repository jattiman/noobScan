/**/
/*
   Recorder.cpp of noobScan, a novice port scanner
   Recorder.cpp records all of the users commands for educational purposes
*/
/**/

#include "Recorder.h"


// class initialization
Recorder::Recorder(){}

// records requests by type
void Recorder::categorizeRequest(std::string userRequest, char category){
    // add request and category to map
    commandMap.insert(std::pair<std::string,char>(userRequest, category));
    
    // add request to record
    recordRequest(userRequest);
    return;
}

// records all user requests in order
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

void Recorder::showTypeHistory(char category){
    
    return;
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
