/**/
/*
 HelpModule.cpp of noobScan, a novice port scanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#include "HelpModule.h"

/**/
/*
HelpModule::HelpModule() HelpModule::HelpModule()
 
NAME
        HelpModule::HelpModule - class constructor
SYNOPSIS
        HelpModule::HelpModule();
DESCRIPTION
        Constructs the help module class, and populates the dictionary based on the loaded file.
RETURNS
        HelpModule class object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
HelpModule::HelpModule(){
    // populate the directory
    populateDirectory();
    return;
}
/*HelpModule();*/

/**/
/*
HelpModule::getValidInput() HelpModule::getValidInput()

NAME
        HelpModule::getValidInput - confirm user input is valid
SYNOPSIS
        int HelpModule::getValidInput(int minNum, int maxNum)
            int minNum --> minimum acceptable number
            int maxNum --> maximum acceptable number
DESCRIPTION
        Function prompts the user for a integer input, handles improperly formatted input, and ensure the integer accepted is sufficient for the prompt given to the user elsewhere in the program. User will be continually prompted until a valid input is entered.
RETURNS
        Integer with confirmed, appropriate user choice.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::outputFeedback() HelpModule::outputFeedback()

NAME
        HelpModule::outputFeedback - outputs user feedback
SYNOPSIS
        void HelpModule::outputFeedback(string feedback);
            feedback --> string holding user feedback
DESCRIPTION
        This function checks to ensure that the user has toggled feedback on. If so, it will output the string to the screen. This saves a few lines of code setting up if statements elsewhere in the program.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::outputFeedback(string feedback){
    if(this->systemFeedback){
        cout << feedback << endl;
    }
    return;
}
/*void outputFeedback(string feedback);*/

/**/
/*
HelpModule::displayDirections() HelpModule::displayDirections()

NAME
        HelpModule::displayDirections - displays opening directions for what the user can do
SYNOPSIS
        void HelpModule::displayDirections();
DESCRIPTION
        This will simply output the opening directions to the user.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::displayDirections(){
    cout << "Type your command, or\n"
        << "\t'help' for directions\n"
        << "\t'settings' to adjust settings\n"
        << "\t'exit' to exit\n"
        << endl;
    return;
}
/*void displayDirections();*/

/**/
/*
HelpModule::displayOptions() HelpModule::displayOptions()

NAME
        HelpModule::displayOptions
SYNOPSIS
        void HelpModule::displayOptions();
DESCRIPTION
        This displays more detailed information about what the user can input and how formatting works.
RETURNS
        Void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::returnInfo() HelpModule::returnInfo()

NAME
        HelpModule::returnInfo - returns help results based on the user request
SYNOPSIS
        void HelpModule::returnInfo(string userString);
            userString --> string containig word that user is looking for
DESCRIPTION
        This function will take in a user string and attempt to find any terms associated with it. It will convert the string to lowercase, then attempt to find it in the help directory map file. If found, it will retrieve the information from the map and output it to the user's screen. If not found, it will inform the user that the term is not defined, and give them information about how they can later define terms in the dictionary, if they'd like.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::returnInfo() HelpModule::returnInfo()

NAME
        HelpModule::returnInfo - returns help results based on the user request
SYNOPSIS
        void HelpModule::returnInfo(int userNum);
            userNum --> integer holding number to look up
DESCRIPTION
        This function retrieves dictionary information about port numbers. It coverts the number to a string, then searches the map for the term. If found, the term will output to the screen. If not found, the user will be informed about how to define the term. It will also talk about ports, in general, to give the user information about port ranges.
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::convertToLowerCase() HelpModule::convertToLowerCase()

NAME
        HelpModule::convertToLowerCase - make string lowercase
SYNOPSIS
        void HelpModule::convertToLowerCase(string & ourString);
            ourString --> string input by user as command. Passed in by reference to be updated.
DESCRIPTION
        This function will take in a user string, convert it to lower case, and return it. C++ does not seem to have a to_lower function like some of the other languages do (natively), so we unfortunately must iterate through the string and make it all lower case, letter by letter.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::convertToLowerCase(string & ourString){
    // TODO: account for non-english character keyboards
    for(auto & alpha: ourString){
        alpha = tolower(alpha);
    }
    return;
}
/*void convertToLowerCase(string & ourString);*/

/**/
/*
HelpModule::populateDirectory() HelpModule::populateDirectory()

NAME
        HelpModule::populateDirectory - creates the dictionary for the program
SYNOPSIS
        void HelpModule::populateDirectory();
DESCRIPTION
        This function uses file manipulation to retrieve the file of terminology and populate a map (for quick future lookups) with all terminology that the program relies upon. This file can be updated, as can the map itself, throughout the program (although doing this would be rare, and relates to a differne function).
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::printFullDirectory() HelpModule::printFullDirectory()

NAME
        HelpModule::printFullDirectory - prints the full dictionary
SYNOPSIS
        void HelpModule::printFullDirectory();
DESCRIPTION
        This function will print all items in the dictionary. Best if the user wishes to load their own smaller dictionaries.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::printFullDirectory(){
    // iterate through entire map
    for(auto terms : helpDirectory){
        // print the dictionary term and definition
        cout << terms.first << ": " << terms.second << endl;
    }
    return;
}
/*void printFullDirectory();*/

/**/
/*
HelpModule::promptToAdd() HelpModule::promptToAdd()

NAME
        HelpModule::promptToAdd - prompts user to add to dictionary
SYNOPSIS
        void HelpModule::promptToAdd();
DESCRIPTION
        This function prompts a user to see if they want to add an item to the dictionary. If they say yes, they are walked through how to add an item to the dictionary. Otherwise, the function returns.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::addToDictionary() HelpModule::addToDictionary()

NAME
        HelpModule::addToDictionary - adds item to dictionary
SYNOPSIS
        void HelpModule::addToDictionary();
DESCRIPTION
        This function is actuated if the user indicates they want to add a term to the dictionary.
        The function gets the new word from the user (or allows them to cancel), allows the user to define the word, then appends it to the dictionary file and map.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::addToHelpDirectory() HelpModule::addToHelpDirectory()

NAME
        HelpModule::addToHelpDirectory - add individual entry to help directory
SYNOPSIS
        void HelpModule::addToHelpDirectory(string word, string definition);
            word        --> string holding user word
            definition  --> string holding definition for word
DESCRIPTION
        This function updates the directory map with a word/definition of the user's choosing
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::addToHelpDirectory(string word, string definition){
    this->helpDirectory[word] = definition;
}
/*void addToHelpDirectory(string word, string definition);*/

/**/
/*
HelpModule::getNewEntry() HelpModule::getNewEntry()

NAME
        HelpModule::getNewEntry - gets a new dictionary term from user
SYNOPSIS
        string HelpModule::getNewEntry();
DESCRIPTION
        This function retrieves a word and definition from the user, and reports back. The user may cancel by entering "cancel" as their word, or by selecting the cancel option after they have a chance to view their entry.
RETURNS
        String concatenating the user's word and definition, to be added to the dictionary for the user to later view.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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

/**/
/*
HelpModule::helpWarning() HelpModule::helpWarning()

NAME
        HelpModule::helpWarning - outputs a warning for when the help function is used incorrectly
SYNOPSIS
        void HelpModule::helpWarning();
DESCRIPTION
        This function outputs a warning for when the help function is used incorrectly.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::helpWarning(){
    cout << "We noticed you're asking for help for more than 1 term. We'll only help out with 1 term at a time, so please ask for each term individually. When multiple terms are requested, word-based terms (non-ports) are favored." << endl;
    return;
}
/*void helpWarning();*/

/**/
/*
HelpModule::setSystemFeedback() HelpModule::setSystemFeedback()

NAME
        HelpModule::setSystemFeedback - turn on and off feedback
SYNOPSIS
        void HelpModule::setSystemFeedback(bool isOn);
            isOn --> bool indicating if feedback is on or off
DESCRIPTION
        Setter function to turn the request for feedback on or off
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void HelpModule::setSystemFeedback(bool isOn){
    this->systemFeedback=isOn;
}
/*void setSystemFeedback(bool isOn);*/

/**/
/*
HelpModule::getSystemFeedback() HelpModule::getSystemFeedback()

NAME
        HelpModule::getSystemFeedback - get system feedback status
SYNOPSIS
        bool HelpModule::getSystemFeedback();
DESCRIPTION
        Returns the systemFeedback class variable.
RETURNS
        Bool indicating if the system feedback is on or off.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool HelpModule::getSystemFeedback(){
    return this->systemFeedback;
}
/*bool getSystemFeedback();*/

