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
    cout << "Type your command, or\n"
        << "\t'help' for directions\n"
        << "\t'settings' to adjust settings\n"
        << "\t'exit' to exit\n"
        << endl;
    return;
}

// tells the user everything they can say
void HelpModule::displayOptions(){
    
    // general help commands overview
    cout << "\nGeneral help commands overview: \n"
        << "\thelp [term]: will give you a definition of a term.\n"
        << "\t\tExample:\n\t\t\thelp port\n\n"
        << "\thelp [scan type]: will tell you how to format your specific scan requests.\n"
        << "\t\tExample:\n\t\t\thelp tcp\n\n\thelp [port number]: will give you general information about a port\n"
        << "\t\tExample:\n\t\t\thelp 22\n"
        << endl;
    
    // scanning overview
    cout << "General scanning commands overview:\n"
        << "\tLooking to scan? Input \"scan [scan type] [scanning destination] [ports or port types]\" at the next prompt.\n"
        << "\t\tExample:\n\t\t\tscan tcp www.google.com 22 4747 88 (to TCP scan www.google.com at ports 22, 4747, and 88.\n\n"
        << "\t\tExample:\n\t\t\tscan syn chat (to syn scan common \"chat\" application ports (IRC, Discord, etc) on 127.0.0.1.\n\n"
        << "\t\tNote: inputting the IP address of your destination is always preferred (plays nicer).\n"
        << endl;
    
    // settings overview
    cout << "Looking to change settings? Input \"settings\" instead of \"help\" at the next prompt. A list of settings that you can manipulate will be displayed:\n"
        << "\toutput: toggles output to screen or file\n"
        << "\tdelay: allows you to set the delay time between scanning ports\n"
        << "\tdictionary: allows you to add items or review the dictionary"
        << endl;
    
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(string userString){
    if(helpDirectory.count(userString)!=0){
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    else if(isdigit(userString[0])){
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n"
            << "\tPorts typically number from 0 to 65535.\n" << endl;
    }
    else{
        cout << "\t" << userString << " is an undefined term! Want to add items to the dictionary? You can do that manually through settings.\n" << endl;
    }
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(int userNum){
    // create placeholder string for int conversion
    string userString = to_string(userNum);
    
    // if the term is in the dictionary, print it
    if(helpDirectory.count(userString)!=0){
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    // if the term isn't in the dictionary, but starts with a number, let them know it's an undefined port
    else if(isdigit(userString[0])){
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n";
        cout << "\tPorts typically number from 0 to 65535.\n\n";
    }
    // otherwise, if it's not in the dictionary, and not a port number.
    else{
        cout << "\t" << userString << " is an undefined term! Want to add items to the dictionary? You can do that manually through settings.\n\n";
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
        tokenHunter=fileLine.find(':');
        dictionaryTerm = fileLine.substr(0,tokenHunter);
        definition = fileLine.substr(tokenHunter+2,string::npos);
        
        // add this to the map
        helpDirectory[dictionaryTerm] = definition;
    }
    
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

void HelpModule::promptToAdd(){
    char userChoice;
    cout << "Would you like to add a term to the dictionary? (y/n) \n\n";
    cin >> userChoice;
    if(userChoice=='y'){
        addToDictionary();
    }
    return;
}

void HelpModule::addToDictionary(){
    // create holders for new word entry
    string ourEntry;
    
    // prompt user to input new entry
    ourEntry=getNewEntry();
    
    // if the user wanted to redefine a term, their entry would have been replaced with "cancel". Inform them of their error and exit.
    if(ourEntry.compare("cancel")==0){
        cout << "We're cancelling your request. Better luck next time!" << endl;
        return;
    }
    
    // otherwise, add the dictionary item to the end of the file
    else{
        // create file holder
        ofstream ourFile;
        
        // open file to append
        //ourFile.open("helpDirectory.txt", ios::out | ios::app);
        ourFile.open("helpDirectory.txt", ios::app);
        
        // confirm no issue with opening
        if(ourFile.good()){
            // append the new word to the dictionary
            ourFile << ourEntry;
            ourFile.close();
        }
        else{
            cout << "File did not open correctly." << endl;
        }
    }
    return;
}

//TODO: figure out why this triggers "You didn't enter a command. Try again" when it returns.
string HelpModule::getNewEntry(){
    // string holder for new dictionary word
    string ourWord;
    
    // string holder for dictionary definition
    string ourDefinition;
    
    // new entry string
    string newEntry;
    
    // holder for user answer
    char userAnswer;
        
    // get the word to define
    cout << "Enter the word you want to define:" << endl;
    
    getline(cin,ourWord);
        
    // check to confirm the word is not already defined
    if(helpDirectory.count(ourWord)>0){
        cout << "This word is already defined. Sorry - these words are set in stone." << endl;
        ourWord.clear();
        return "cancel";
    }
    
    // get the definition
    cout << "Enter the definition for " << ourWord <<":" << endl;
    
    getline(cin,ourDefinition);
    
    newEntry=ourWord+": "+ourDefinition;
    
    cout << "Are you sure you want to input the following entry? (y/n) \n\t" << newEntry << endl;
    
    cin >> userAnswer;
    
    if(userAnswer=='y'){
        return newEntry;
    }
    else{
        ourWord.clear();
        ourDefinition.clear();
        newEntry.clear();
        return "cancel";
    }
}

void HelpModule::helpWarning(){
    cout << "We noticed you're asking for help for more than 1 term. We'll only help out with 1 term at a time, so please ask for each term individually. When multiple terms are requested, word-based terms (non-ports) are favored." << endl;
    return;
}
