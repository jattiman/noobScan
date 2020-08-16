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
        // if found, output the commands under that history type. Use a little iomanip to make things a lot easier to track
        cout << setw(60) << left << "Command" << setw(30) << right << " success: (g)ood/(f)ail\n";
        for(auto & element: mapIt->second){
            cout << setfill('_') << setw(60) << left << element.first
                << setfill('_') << setw(30) << right << element.second << "\n";
        }
    }
    else{
        cout << "No such history exists.\n";
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
    
    int userInput;
    string ourEnvironment;
    
    cout << "This file is set to save to the desktop as \"commandHistory.txt\". Would you prefer to place it elsewhere?"
        << "\n\t1. Desktop is fine."
        << "\n\t2. Elsewhere (I'll enter my own folder path).\n";
    
    cout << ">: ";
    while(!(cin >> userInput) || userInput < 1 || userInput > 2){
        cin.clear();
        cin.ignore(500,'\n');
        cout << "Please select a valid option.\n>: " << endl;
    }
    
    if(userInput == 1){
        // string to hold home directory file path
        ourEnvironment = getenv("HOME");
        
        if(ourEnvironment.empty()){
            // TODO: allow the user to write their own string
            cout << "Error retrieving home environment. Abandonining file creation.\n";
            return;
        }
        
        ourEnvironment+="/Desktop/commandHistory.txt";
    }
    else{
        cout << "Please enter your folder path (do not include file name or extension):\n";
        getline(cin,ourEnvironment);
        if(ourEnvironment.empty()){
            flush(cout);
            getline(cin,ourEnvironment);
        }
        cout << "You entered: " << ourEnvironment << endl;
        ourEnvironment+="/commandHistory.txt";
    }
    ofstream historyFile(ourEnvironment);
    
    // confirm file opened correctly
    if(historyFile.fail()){
        cout << "File creation issue.\n";
        return;
    }
    
    // pull time to output to file
    time_t ourTime = time(0);
    
    // convert into structure
    struct tm * readableTime = localtime(&ourTime);
    
    // output info to file
    historyFile << "Outputting history on " << 1 + readableTime->tm_mon << "/" << readableTime->tm_mday << "/" << 1900 + readableTime->tm_year << ", at " << readableTime->tm_hour << ":" << readableTime->tm_min << "\n";
    historyFile << "\n\n---SCAN commands ...\n";
    historyFile << writeHistoryByType('s');
    historyFile << "\n\n---SETTINGS commands ...\n";
    historyFile << writeHistoryByType('v');
    historyFile << "\n\n---HELP commands ...\n";
    historyFile << writeHistoryByType('h');
    historyFile << "\n\n---IP CHECK commands ...\n";
    historyFile << writeHistoryByType('i');
    historyFile << "\n\n---ALL commands in order...\n";
    historyFile << writeHistoryByType();
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
