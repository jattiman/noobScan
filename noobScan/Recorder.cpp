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
Recorder::Recorder() Recorder::Recorder()
 
NAME
        Recorder::Recorder - class constructor
SYNOPSIS
        Recorder::Recorder();
DESCRIPTION
        Class constructor. Sets the recorder to be on by default.
RETURNS
       Recorder class object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
Recorder::Recorder(){
    this->isRecorderOn=true;
}
/*Recorder();*/

/**/
/*
Recorder::Recorder() Recorder::Recorder()

NAME
        Recorder::Recorder - class constructor
SYNOPSIS
        Recorder::Recorder(bool isOn);
            isOn --> bool value to initialize recorder as on or off
DESCRIPTION
        Additional class constructor to toggle recorder settings
RETURNS
        Recorder class object.
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
Recorder::categorizeOutcome() Recorder::categorizeOutcome()

NAME
        Recorder::categorizeOutcome - categorizes user input, type, and outcome
SYNOPSIS
        void Recorder::categorizeOutcome(string userRequest, char outcome, char category);
            userRequest --> String that the user put in as their command
            outcome     --> Outcome of the request (good, fail)
            category    --> Category of their request (scan, help, settings, etc)
DESCRIPTION
        This function will record the commands of the user and categorize them while the recorder is on.
        Although I would have liked to utilize SQL integrations into C++, I did not have the time to learn that language or how to integrate it. Instead, this information is placed into various unordered maps containing integers, vectors, and pairs (depending on the map). Prior to this project, I had never worked with pairs, or unordered_maps. It was a fun learning experience.
RETURNS
        Void - no return.
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
Recorder::tallyUpdate() Recorder::tallyUpdate()

NAME
        Recorder::tallyUpdate - tally the request types received
SYNOPSIS
        void Recorder::tallyUpdate(char category);
            category --> The category of the map to be incremented
DESCRIPTION
        When a new user command comes in and is categorized, this function will increment the number of times the user utilized that category in their commands. These stats are used later.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void Recorder::tallyUpdate(char category){
    // add request to the map, and increment as needed
    tallyMap[category]+=1;
    return;
}
/*void tallyUpdate(char category);*/

/**/
/*
Recorder::tallyRetrieval() Recorder::tallyRetrieval()

NAME
        Recorder::tallyRetrieval - retrieve the count of tallies based on type
SYNOPSIS
        string Recorder::tallyRetrieval(char category)
            category --> the category of the tallies that will be retrieved
DESCRIPTION
        This retrieves the number of times that a user attempted to utilize a category of the program (help, settings, scan, etc). It uses a stringstream variable to convert the number to a string easily, and then returns this value as a string.
RETURNS
        String representing the tally.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
Recorder::clearHistory() Recorder::clearHistory()

NAME
        Recorder::clearHistory - clears the history of the user fully.
SYNOPSIS
        void Recorder::clearHistory();
DESCRIPTION
        This function clears the user's history fully, essentially negating the recorder, if needed.
RETURNS
        Void - no return.
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
Recorder::showHistory() Recorder::showHistory()

NAME
        Recorder::showHistory - shows user history
SYNOPSIS
        void Recorder::showHistory(char historyType);
            historyType --> history type we're looking up
DESCRIPTION
        This function will pull up the user's command history.
        The function intakes a character denoting the history type we're looking up (default value is 'a' for all history), and then the information is output to the screen. Iomanip is also used here to help format it to the screen a bit better.
        Interesting note with this function, related to Xcode: C++17 is not turned on by default, so the auto iterator loop below currently gives me a warning that the formatting is a C++17 extension. I wanted to try out some C++17 standards in this program, and it seems to run without issue, so I'm keeping it in.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
writeHistoryByType() writeHistoryByType()

NAME
        Recorder::writeHistoryByType - assists in writing the history to the file
SYNOPSIS
        string Recorder::writeHistoryByType(char historyType);
            historyType --> char representing the history type being looked for
DESCRIPTION
        This function uses a stringstream to format the outcome of the command map into a string, which is returned to be placed into a file. It uses iomanip to format it a bit differently than normal output, and iterates through a map to collect the needed output. C++17 standards used to access the map. If the string is not populated by history (no history of that category present), placeholder text will instead be copied to the string.
RETURNS
        String holding the map output.
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
Recorder::moveHistoryToFile() Recorder::moveHistoryToFile()
 
NAME
        Recorder::moveHistoryToFile - copy all commands to file (class controls file)
SYNOPSIS
        void Recorder::moveHistoryToFile();
DESCRIPTION
        This function adds all commands to a file for the user to review. It starts by giving the user the option of having the file save on their desktop, but also allows the user to dictate where the file will save.
        Once a file destination is chosen, it will create a file, retrieve the appropriate command history and stats, then add this information to the file.
        To ensure that the user knows this is the most updated file, the system time will be formatted and output to the top of the file.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
Recorder::setRecorderStatus() Recorder::setRecorderStatus()

NAME
        Recorder::setRecorderStatus - set the recorder on/off status
SYNOPSIS
        void Recorder::setRecorderStatus(bool recorderStatus);
            recorderStatus --> bool signifying if the recorder is to be turned on or off
DESCRIPTION
        This function will set the recorder to be on or off.
RETURNS
        Void - nothing returned.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void Recorder::setRecorderStatus(bool recorderStatus){
    this->isRecorderOn = recorderStatus;
    return;
}
/*void setRecorderStatus(bool recorderStatus);*/

/**/
/*
Recorder::getRecorderStatus() Recorder::getRecorderStatus()

NAME
        Recorder::getRecorderStatus - returns if the recorder is on or off
SYNOPSIS
        bool Recorder::getRecorderStatus();
DESCRIPTION
        Returns bool variable indicating if the recorder is on or off
RETURNS
        A bool variable indicating if the recorder is on or off.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool Recorder::getRecorderStatus(){
    return this->isRecorderOn;
}
/*bool getRecorderStatus();*/
