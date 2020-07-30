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
    
    // add request to the overall record
    commandMap['a'].emplace_back(userRequest);
    
    return;
}


// purges history fully
void Recorder::clearHistory(){
    this->commandMap.clear();
    return;
}

// shows all user history
void Recorder::showHistory(char historyType){
    // look for the type of history that the user wants to print (default is all history)
    for(auto commandMapIterator = this->commandMap.cbegin(); commandMapIterator != this->commandMap.cend(); ++commandMapIterator){
        
        // if history type found, print out the vector of the commands under that history
        if(commandMapIterator->first == historyType){
            for(auto vecIt = commandMapIterator->second.cbegin(); vecIt != commandMapIterator->second.cend(); vecIt++){
                std::cout << *vecIt << std::endl;
            }
            // exit the loop to avoid iterating through the rest of the map
            break;
        }
    }
}

void Recorder::showTypeHistory(char category){
    unordered_map<char, vector<string>>:: iterator ourItr;
    for(ourItr = commandMap.begin(); ourItr != commandMap.end(); ourItr++){
        if(ourItr->first == category){
            cout << "FUDGE" << endl;
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
void Recorder::setRecorderStatus(bool recorderStatus){
    this->isRecorderOn = recorderStatus;
    return;
}

// returns if the recorder is on or off
bool Recorder::getRecorderStatus(){
    return this->isRecorderOn;
}
