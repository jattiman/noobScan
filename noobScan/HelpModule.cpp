/**/
/*
 HelpModule.cpp of noobScan, a novice port scanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#include "HelpModule.h"

HelpModule::HelpModule(){
    // populate the directory
    populateDirectory();
    return;
}
/*HelpModule();*/

int HelpModule::getValidInput(int minNum, int maxNum){
    int userInput;
    cout << ">: ";
    while(!(cin >> userInput) || userInput < minNum || userInput > maxNum){
        cin.clear();
        cin.ignore(500,'\n');
        cout << "Please select a valid option." << endl;
    }
    return userInput;
}
/*int getValidInput(int minNum=0, int maxNum=65535);*/

void HelpModule::outputFeedback(string feedback){
    if(this->systemFeedback){
        cout << feedback << endl;
    }
    return;
}
/*void outputFeedback(string feedback);*/

// displays opening prompt for the user to select info for
void HelpModule::displayDirections(){
    cout << "Type your command, or\n"
        << "\t'help' for directions\n"
        << "\t'settings' to adjust settings\n"
        << "\t'exit' to exit\n"
        << endl;
    return;
}
/*void displayDirections();*/

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
        << "\t\tExample:\n\t\t\tscan tcp www.google.com 22 4747 88 (to TCP scan www.google.com at ports 22, 4747, and 88).\n\n"
        << "\t\tExample:\n\t\t\tscan syn chat (to syn scan common \"chat\" application ports (IRC, Discord, etc) on 127.0.0.1).\n\n"
        << "\t\tNote: inputting the IP address of your destination is always preferred (plays nicer). Looking for the IP address to a URL? Enter \"ipcheck\" as a command.\n"
        << endl;
    
    // settings overview
    cout << "Looking to change settings? Input \"settings\" instead of \"help\" at the next prompt. A list of settings that you can manipulate will be displayed.\n";
    
    return;
}
/*void displayOptions();*/

// returns help results based on the user request
void HelpModule::returnInfo(string userString){
    
    // ensure the string is lowercase to minimize errors
    convertToLowerCase(userString);
    
    // confirm the item is in the dictionary
    if(helpDirectory.count(userString)!=0){
        
        // print out the word and definition, formatted to screen
        cout << "\t" << helpDirectory.find(userString)->first << ": ";
        cout << helpDirectory.find(userString)->second << endl << endl;
    }
    
    // if the term isn't defined, and is a number
    else if(isdigit(userString[0])){
        
        // explain that the port is undefined
        cout << "\t" << userString << " is undefined. It looks like you may be searching for port. If so, this port is unassigned.\n"
            << "\tPorts typically number from 0 to 65535.\n" << endl;
    }
    
    // if the term isn't defined and isn't a number
    else{
        
        // display generic "undefined term" script, and direct user to add the item in the future if they wish
        cout << "\t" << userString << " is an undefined term! Want to add items to the dictionary? You can do that manually through settings.\n" << endl;
    }
    return;
}
/*void returnInfo(string userString);*/

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
/*void returnInfo(int userNum);*/

// make string lowercase
void HelpModule::convertToLowerCase(string & ourString){
    // TODO: account for non-english character keyboards
    for(auto & alpha: ourString){
        alpha = tolower(alpha);
    }
    return;
}
/*void convertToLowerCase(string & ourString);*/

void HelpModule::populateDirectory(){
    
    // holds each line of the file (once we get to it)
    string fileLine;
    
    // holds the word for the dictionary entry
    string dictionaryTerm;
    
    // holds the definition for the dictionary entry
    string definition;
    
    // holds the position of the token separating the word from the definition
    unsigned long tokenHunter=0;
    
    // open dictionary file for input
    // Note: the full file path needs to be listed, for now, to run from an executable outside of Xcode. Relative paths will work in Xcode, but not when running from terminal/console
    ifstream ourFile("/Users/jatti/Documents/noobScan/noobScan/helpDirectory.txt", ifstream::in);
    
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
        
        // when pulling the definition, we're accounting for the space after the token to avoid moving whitespace/symbols
        definition = fileLine.substr(tokenHunter+2,string::npos);
        
        // add this to the map
        helpDirectory[dictionaryTerm] = definition;
    }
    
    // close your file
    ourFile.close();
    return;
}
/*void populateDirectory();*/

// prints the full dictionary
void HelpModule::printFullDirectory(){
    // iterate through entire map
    for(auto terms : helpDirectory){
        // print the dictionary term and definition
        cout << terms.first << ": " << terms.second << endl;
    }
    return;
}
/*void printFullDirectory();*/

void HelpModule::promptToAdd(){
    char userChoice;
    cout << "Would you like to add a term to the dictionary? (y/n) \n\n";
    cin >> userChoice;
    if(userChoice=='y'){
        addToDictionary();
    }
    return;
}
/*void promptToAdd();*/

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
        //ourFile.open("helpDirectory.txt", ios::app);
        ourFile.open("/Users/jatti/Documents/noobScan/noobScan/helpDirectory.txt", ios::app);
        
        // confirm no issue with opening
        if(ourFile.good()){
            // append the new word to the dictionary file
            ourFile << ourEntry << endl;
            ourFile.close();
            
            // TODO: add to our helpDirectory
            cout << "Entry added successfully." << endl;
        }
        else{
            cout << "File did not open correctly." << endl;
        }
    }
    return;
}
/*void addToDictionary();*/

// add individual entry to help directory
void HelpModule::addToHelpDirectory(string word, string definition){
    this->helpDirectory[word] = definition;
}
/*void addToHelpDirectory(string word, string definition);*/

// gets a new dictionary term from user
string HelpModule::getNewEntry(){
    cin.ignore(256,'\n');
    
    // string holder for new dictionary word
    string ourWord="";
    
    // string holder for dictionary definition
    string ourDefinition="";
    
    // new entry string
    string newEntry="";
    
    // holder for user answer
    int userAnswer;
        
    // get the word to define
    cout << "Enter the word you want to define:" << endl;
    
    getline(cin,ourWord);
        
    // check to confirm the word is not already defined
    if(helpDirectory.count(ourWord)>0){
        cout << "This word is already defined. Sorry." << endl;
        ourWord.clear();
        return "cancel";
    }
    
    // get the definition
    cout << "Enter the definition for " << ourWord <<":" << endl;
    
    getline(cin,ourDefinition);
    
    newEntry=ourWord+": "+ourDefinition;
    
    cout << "Are you sure you want to input the following entry?\n\t" << newEntry << endl;
    cout << "\t1. Yes\n"
    << "\t2. No (cancel entry)" << endl;
    
    userAnswer = getValidInput(1,2);
    
    if(userAnswer==1){
        // add to map
        this->addToHelpDirectory(ourWord, ourDefinition);
        
        // return to add to file
        return newEntry;
    }
    else{
        ourWord.clear();
        ourDefinition.clear();
        newEntry.clear();
        return "cancel";
    }
}
/*string getNewEntry();*/



void HelpModule::helpWarning(){
    cout << "We noticed you're asking for help for more than 1 term. We'll only help out with 1 term at a time, so please ask for each term individually. When multiple terms are requested, word-based terms (non-ports) are favored." << endl;
    return;
}
/*void helpWarning();*/


// turn on and off feedback
void HelpModule::setSystemFeedback(bool isOn){
    this->systemFeedback=isOn;
}
/*void setSystemFeedback(bool isOn);*/

// check if feedback is on
bool HelpModule::getSystemFeedback(){
    return this->systemFeedback;
}
/*bool getSystemFeedback();*/

