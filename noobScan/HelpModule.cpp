/**/
/*
   HelpModule.cpp of noobScan, a novice port scanner
   HelpModule.cpp provides information to the user about commands
*/
/**/

#include "HelpModule.h"

HelpModule::HelpModule(){
    // populate the directory
    populateDirectory();
    return;
}

// displays opening prompt for the user to select info for
void HelpModule::displayDirections(){
    cout << "Type your command, or\n\t'help' for directions\n\t'settings' to adjust settings\n\t'exit' to exit\n";
    return;
}

// tells the user everything they can say
void HelpModule::displayOptions(){
    cout << "Detailed commands: " << std::endl;
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(string userString){
    if(helpDirectory.count(userString)!=0){
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    else if(isdigit(userString[0])){
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n";
        cout << "\tPorts typically number from 0 to 65535.\n\n";
    }
    else{
//        char userOption;
//        cout << "\t" << userString << " is an undefined term! Would you like to add it to the dictionary? (y/n) \n\n";
//        cin >> userOption;
//        if(userOption=='y'){
//            addToDictionary();
//        }
        this->promptToAdd(userString);
    }
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(int userNum){
    // create placeholder string for int conversion
    string userString = to_string(userNum);
    
    if(helpDirectory.count(userString)!=0){
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    else if(isdigit(userString[0])){
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n";
        cout << "\tPorts typically number from 0 to 65535.\n\n";
    }
    else{
//        char userOption;
//        cout << "\t" << userString << " is an undefined term! Would you like to add it to the dictionary? (y/n) \n\n";
//        cin >> userOption;
//        if(userOption=='y'){
//            addToDictionary();
//        }
        
        this->promptToAdd(userString);
    }
    return;
}

void HelpModule::populateDirectory(){
    // holds each line of the file (once we get to it)
    string fileLine;
    string dictionaryTerm;
    string definition;
    unsigned long tokenHunter=0;
    
    // open dictionary file for input
    ifstream ourFile("helpDirectory.txt", ifstream::in);
    
    // confirm file opened correctly. Exit if not.
    if(!ourFile.good()){
        cout << "Dictionary file issue: cannot open.\n";
        return;
    }
    
    // iterate through the file, and assign definitions to map accordingly
    while(getline(ourFile, fileLine)){
        // instead of using regex (robust), we'll manually hunt for the delimiter and split each file line into word and definition
//        cout << fileLine << endl;
        tokenHunter=fileLine.find(':');
        dictionaryTerm = fileLine.substr(0,tokenHunter);
        definition = fileLine.substr(tokenHunter+2,string::npos);
//        cout << dictionaryTerm << ":" << definition << endl;
        
        // add this to the map
        helpDirectory[dictionaryTerm] = definition;
    }
    
//    cout << "Dictionary updated successfully.\n";
    //this->printFullDirectory();
    // close your file
    ourFile.close();
    return;
}

// prints the full dictionary
void HelpModule::printFullDirectory(){
    // iterate through entire map
    for(auto terms : helpDirectory){
        // print the dictionary term and definition
        cout << terms.first << ": " << terms.second << endl;
    }
    return;
}

void HelpModule::promptToAdd(string userWord){
    char userChoice;
    cout << "\t" << userWord << " is an undefined term! Would you like to add it to the dictionary? (y/n) \n\n";
    cin >> userChoice;
    if(userChoice=='y'){
        addToDictionary();
    }
    return;
}

void HelpModule::addToDictionary(){
    // create holders for our file and new word entry
    ofstream ourFile;
    string ourEntry;
    
    // prompt user to input new entry - keep attempting if they make a mistake
    do{
        ourEntry=getNewEntry();
    }while(ourEntry.compare("error")!=0);
    
    // if the user wanted to redefine a term, cancel (not allowed)
    if(ourEntry.compare("cancel")==0){
        return;
    }
    // otherwise, add the dictionary item to the end of the file
    else{
        //ourFile.open("helpDirectory.txt", ios::out | ios::app);
        ourFile.open("helpDirectory.txt", ios::app);
        if(ourFile.good()){
            ourFile << ourEntry;
            ourFile.close();
        }
    }
    return;
}

string HelpModule::getNewEntry(){
    string ourWord;
    string ourDefinition;
    string newEntry;
    
    char userAnswer;
    
    // get the word to define
    cout << "Enter the word you want to define:\n\t";
    getline(cin,ourWord);
    //TODO: work on getline issues - move map count term to prompt
    // check to confirm the word is not already defined
    if(helpDirectory.count(ourWord)>0){
        cout << "This word is already defined. Sorry - these words are set in stone.\n";
        return "cancel";
    }
    
    // get the definition
    cout << "Enter the definition for your word:\n\t";
    getline(cin,ourDefinition);
    newEntry=ourWord+ourDefinition;
    
    cout << "Are you sure you want to input the following entry? (y/n) \n\t" << newEntry << endl;
    
    cin >> userAnswer;
    
    if(userAnswer=='y'){
        return newEntry;
    }
    else{
        return "error";
    }
}
