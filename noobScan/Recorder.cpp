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
void Recorder::categorizeRequest(string userRequest, char category){
    
    // add request and category to map
    commandMap[category].emplace_back(userRequest);
    
    // if the "all" category was not entered
    if(category!='a'){
        // add request to the record of all commands
        commandMap['a'].emplace_back(userRequest);
    }
    return;
}

void Recorder::categorizeOutcome(string userRequest, char outcome, char category){
    // add request and outcome to map, by category
    outcomeMap[category].emplace_back(make_pair(userRequest,outcome));
    
    // add request to the overall record
    if(category!='a'){
        // add request to the record of all commands
        outcomeMap['a'].emplace_back(make_pair(userRequest, outcome));
    }
}


// purges history fully
void Recorder::clearHistory(){
    this->outcomeMap.clear();
    return;
}

// shows user history
void Recorder::showHistory(char historyType){

    // look for the type of history that the user wants to print (default is all history). Using C++17 for if initialization, here.
    if(auto mapIt = this->outcomeMap.find(historyType); mapIt!=this->outcomeMap.end()){
        // if found, output the commands under that history type. Use a little iomanip to make things a little easier to track
        cout << setw(60) << left << "Command" << setw(30) << right << " success: (g)ood/(f)ail\n";
        for(auto & element: mapIt->second){
            cout << setfill('_') << setw(60) << left << element.first << setfill('_') << setw(30) << right << element.second << "\n";
        }
    }
    
}

// writes history to a file by type
string Recorder::writeHistoryByType(char historyType){
    stringstream ourStream;
    string outPut;
    // look for the type of history that the user wants to print (default is all history). Using C++17 for if initialization, here.
    if(auto mapIt = this->outcomeMap.find(historyType); mapIt!=this->outcomeMap.end()){
        // if found, output the commands under that history type. Use a little iomanip to make things a little easier to track
        ourStream << setw(60) << left << "Command" << setw(30) << right << " success: (g)ood/(f)ail\n";
        for(auto & element: mapIt->second){
            ourStream << setfill('_') << setw(60) << left << element.first << setfill('_') << setw(30) << right << element.second << "\n";
        }
    }
    else{
        ourStream << "No such history exists.\n";
    }
    outPut = ourStream.str();
    return outPut;
}

// copy all commands to file (class controls file)
void Recorder::moveHistoryToFile(){
    
    // string to hold home directory file path
    string ourEnvironment(getenv("HOME"));
    
    if(ourEnvironment.empty()){
        // TODO: allow the user to write their own string
        cout << "Error retrieving home environment. Abandonining file creation.\n";
        return;
    }
    
    ourEnvironment+="/Desktop/commandHistory.txt";
    ofstream historyFile(ourEnvironment);
    
    // confirm file opened correctly
    if(historyFile.fail()){
        cout << "File creation issue.\n";
        return;
    }
    
    historyFile << "---SCAN commands ...\n";
    historyFile << writeHistoryByType('s');
    historyFile << "--------------------\n";
    historyFile << "---SETTINGS commands ...\n";
    historyFile << writeHistoryByType('v');
    historyFile << "--------------------\n";
    historyFile << "---HELP commands ...\n";
    historyFile << writeHistoryByType('h');
    historyFile << "--------------------\n";
    historyFile << "---IP CHECK commands ...\n";
    historyFile << writeHistoryByType('i');
    historyFile << "--------------------\n";
    historyFile << "---ALL commands in order...\n";
    historyFile << writeHistoryByType();
    historyFile << "--------------------\n";
    historyFile.close();
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
