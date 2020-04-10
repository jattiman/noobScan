/**/
/*
   Recorder.cpp of noobScan, a novice port scanner
   Recorder.cpp records all of the users commands for educational purposes
*/
/**/

#include "Recorder.h"


// class initialization
Recorder::Recorder(){
    this->isRecorderOn=true;
}

// records requests by type
void Recorder::categorizeRequest(std::string userRequest, char category){
    
    // add request and category to map
    commandMap[category].emplace_back(userRequest);
    
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
    std::unordered_map<char, std::vector<std::string>>:: iterator ourItr;
    for(ourItr = commandMap.begin(); ourItr != commandMap.end(); ourItr++){
        if(ourItr->first == category){
            std::cout << "FUCK" << std::endl;
        }
    }
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

// set the recorder on/off status
void Recorder::setRecorderStatus(bool recorderOn){
    this->isRecorderOn = recorderOn;
    return;
}

// returns if the recorder is on or off
bool Recorder::getRecorderStatus(){
    return this->isRecorderOn;
}
