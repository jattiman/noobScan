/**/
/*
   HelpModule.cpp of noobScan, a novice port scanner
   HelpModule.cpp provides information to the user about commands
*/
/**/

#include "HelpModule.h"

HelpModule::HelpModule(){
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
void HelpModule::returnInfo(std::string userString){
    if(helpDirectory.count(userString)!=0){
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    else if(isdigit(userString[0])){
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n";
        cout << "\tPorts typically number from 0 to 65535.\n\n";
    }
    else{
        cout << "\t" << userString << " is an undefined term! Would you like to add it to the dictionary?\n\n";
        //TODO: dictionary add feature
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
    return;
}

// prints the full dictionary
void HelpModule::printFullDirectory(){
    // iterate through entire map
    for(auto terms : helpDirectory){
        // print the dictionary term and definition
        cout << terms.first << ": " << terms.second << endl;
    }
}
