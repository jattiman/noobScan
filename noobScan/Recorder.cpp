/**/
/*
 Recorder.cpp of noobScan, a novice port scanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#include "Recorder.h"

/**/
/*
[paste function here without anything in parantheticals]

NAME

        [function name w/o parantheticals and brief descriptions on one line]

SYNOPSIS

        [full function name with parantheticals filled in]
        [breakdown of what each variable in the parantheticals does]

DESCRIPTION

        [full description here - as long as makes sense]

RETURNS

        [obvious]

AUTHOR

        John Atti

DATE

        6:00 PM 8/16/2020

*/
/**/
// class initialization
Recorder::Recorder(){
    this->isRecorderOn=true;
}
/*Recorder();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
Recorder::Recorder(bool isOn){
    this->isRecorderOn=isOn;
}
/*Recorder(bool isOn);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void Recorder::categorizeOutcome(string userRequest, char outcome, char category){
    // add request and outcome to map, by category
    outcomeMap[category].emplace_back(make_pair(userRequest,outcome));
    
    // update separate tally for the request as well as the outcome, which will allow us to view stats later
    tallyUpdate(category);
    
    // confirm there is no duplicate request (we don't want to double count fails)
    if(category!=outcome){
        tallyUpdate(outcome);
    }
    
    // add request to the overall record
    if(category!='a'){
        // add request to the record of all commands
        outcomeMap['a'].emplace_back(make_pair(userRequest, outcome));
    }
}
/*void categorizeOutcome(string userRequest, char outcome, char category='x');*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// tally the request types received
void Recorder::tallyUpdate(char category){
    // add request to the map, and increment as needed
    tallyMap[category]+=1;
    //cout << "\t\tnew tally for " << category << " is " <<tallyMap[category] << endl;
    return;
}
/*void tallyUpdate(char category);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// retrieve the count of tallies based on type
string Recorder::tallyRetrieval(char category){
    stringstream ourStream;
    string tallyString;
    tallyString.clear();
    
    // search for a map entry with the category value
    // if found,
    if(tallyMap.count(category)>0){
        // add tally value to stream
        ourStream << tallyMap[category];
    }
    // if not found,
    else{
        // insert a 0 count
        ourStream << "0";
    }
    
    // convert stream to string
    tallyString=ourStream.str();

    
    return tallyString;
}
/*string tallyRetrieval(char category);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// purges history fully
void Recorder::clearHistory(){
    this->outcomeMap.clear();
    return;
}
/*void clearHistory();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void showHistory(char historyType='a');*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*string writeHistoryByType(char historyType='a');*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// copy all commands to file (class controls file)
void Recorder::moveHistoryToFile(){
    
    // int variable holding user selection choice, for prompts
    int userInput;
    
    // string that will hold the filePath
    string ourFilePath;
    
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
        ourFilePath = getenv("HOME");
        
        if(ourFilePath.empty()){
            // TODO: allow the user to write their own string
            cout << "Error retrieving home environment. Abandonining file creation.\n";
            return;
        }
        
        ourFilePath+="/Desktop/commandHistory.txt";
    }
    else{
        cout << "Please enter your folder path (do not include file name or extension):\n";
        getline(cin,ourFilePath);
        if(ourFilePath.empty()){
            flush(cout);
            getline(cin,ourFilePath);
        }
        cout << "You entered: " << ourFilePath << endl;
        ourFilePath+="/commandHistory.txt";
    }
    ofstream historyFile(ourFilePath);
    
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
    historyFile << "\n\nSCAN commands:\n";
    historyFile << writeHistoryByType('s');
    historyFile << "\nTotal scan commands: " << tallyRetrieval('s');
    historyFile << "\n\nSETTINGS commands:\n";
    historyFile << writeHistoryByType('v');
    historyFile << "\nTotal settings commands: " << tallyRetrieval('v');
    historyFile << "\n\nHELP commands:\n";
    historyFile << writeHistoryByType('h');
    historyFile << "\nTotal help commands: " << tallyRetrieval('h');
    historyFile << "\n\nIP CHECK commands:\n";
    historyFile << writeHistoryByType('i');
    historyFile << "\nTotal IP Check commands: " << tallyRetrieval('i');
    historyFile << "\n\nALL commands in order:\n";
    historyFile << writeHistoryByType();
    historyFile << "\nTotal successful commands: " << tallyRetrieval('g') << "\n";
    historyFile << "\nTotal failed and partially failed commands: " << tallyRetrieval('f') << "\n";
    
    // close the file
    historyFile.close();
}
/*void moveHistoryToFile();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// set the recorder on/off status
void Recorder::setRecorderStatus(bool recorderStatus){
    this->isRecorderOn = recorderStatus;
    return;
}
/*void setRecorderStatus(bool recorderStatus);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// returns if the recorder is on or off
bool Recorder::getRecorderStatus(){
    return this->isRecorderOn;
}
/*bool getRecorderStatus();*/
