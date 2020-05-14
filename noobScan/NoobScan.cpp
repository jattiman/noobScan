//
//  NoobScan.cpp
//  noobScan
//
//  Created by John Atti on 4/9/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "NoobScan.h"

NoobScan::NoobScan(){
    this->ourHelper = new HelpModule();
    this->userRecorder = new Recorder();
    this->ourScanner = new ScanAddress();
    this->ourTCPScan = new TCPScanner();
    this->ourResult.clear();
    this->ourCommand.clear();
}

// output logo
// http://patorjk.com/software/taag/#p=display&f=Ogre&t=NoobScan
void NoobScan::printLogo(){
    cout<< endl
        << "     __            _     __                 \n"
        << "  /\\ \\ \\___   ___ | |__ / _\\ ___ __ _ _ __  \n"
        << " /  \\/ / _ \\ / _ \\| '_ \\\\ \\ / __/ _` | '_ \\  \n"
        << "/ /\\  / (_) | (_) | |_) |\\ \\ (_| (_| | | | |\n"
        << "\\_\\ \\/ \\___/ \\___/|_.__/\\__/\\___\\__,_|_| |_|\n"
        << "                                            \n"
        << endl;
    
}

// give user the welcome text
void NoobScan::welcomeText(){
    cout << "Welcome to NoobScan, the newbie port scanner!\n"
    << "Please scan responsibly, and be ready to learn a thing or two.\n\n";
    return;
}

//initial user prompt (only shown at startup)
void NoobScan::initialPrompt(){
    // print our logo
    this->printLogo();
    
    // welcome the user
    this->welcomeText();
    
    // correct usage check
    this->systemCheck();
    
    // display the directions
    ourHelper->displayDirections();
    
    // act on user commands
    this->intakeCommands();
}

void NoobScan::systemCheck(){
    cout << "Let's make sure you won't run into any trouble.\n\n";
    
    // confirm user has admin rights
    this->adminCheck();
    
    // check user operating system for compatibility
    this->opCheck();
    
    // mentally prepare user
    cout << "Please choose your next commands carefully ... \n\n";
}

void NoobScan::adminCheck(){
    uid_t ourID = getuid();
    //bool isAdmin = false;

    struct passwd *userInfo = getpwuid(ourID);
    cout << "Let's see if we can learn something today, " << userInfo->pw_gecos << " (neat username).\n\n";
    
    cout << "Checking admin privileges ... \n\n";

    struct group *adminCheck = getgrnam("admin");
    
    while(*adminCheck->gr_mem != NULL) {
        
        if (strcmp(userInfo->pw_name, *adminCheck->gr_mem) == 0) {
            this->isAdmin=true;
        }
        adminCheck->gr_mem++;
    }
    
    if(this->isAdmin){
        cout << "\tLooks like you have admin rights on this account.\n\tThat's a good thing.\n";
    }
    else{
        cout <<"\tYou don't seem to be an admin.\n\tYou can try running these commands, but you will run into trouble on some scans.\n\n\tStill, feel free to use this as a helper tool, to learn more about the wonderful world of port scanning.\n\n";
    }
}

void NoobScan::opCheck(){
    cout << "Checking operating system...\n\n";
    
    #ifdef _WIN32
        cout << "\tWindows, huh? This program is not going to run well for you. Sorry.\n\n"
    #elif __unix__
        cout << "\tLooks like you're using a unix system, but not OSX.\n\tCertain scans may not work for you.\n\n"
    #elif __APPLE__
        cout << "\tYou're using an OSX variant. That's what this program was made for, so you should be ok!\n\n";
    #endif
}

// intakes all commands from user - main function of this class
void NoobScan::intakeCommands(){
    while(true){
        // prompt user for command
        this->ourCommand = promptUser();
        
        // act on the command
        this->commandResponse(ourCommand);
        
        if(ourCommand.compare("exit")==0){
            return;
        }
        
        // if Recorder is on
        if(userRecorder->getRecorderStatus()){
        
            // categorize the request
            userRecorder->categorizeRequest(ourCommand);
            
            // categorize the answer
        
        }
        // otherwise, just output the answer
        cout << this->ourResult;
        
    }
    return;
}

// prompt user
string NoobScan::promptUser(){
    // prompt user for their command
    cout << ">: ";
    string userCommand;
    getline(cin, userCommand);
    return userCommand;
}

// respon to user commands by initiating the appropriate functions
void NoobScan::commandResponse(string userCommand){
    // clear result string
    this->ourResult.clear();
    
    // intake command
    string commandManip = userCommand;
    
    // categorize base request
    inspectArgs(commandManip);
    
    // update result string with answer to request
    
    return;
}

void NoobScan::inspectArgs(string userCommand){
    // ensure any previous commands are empty
    clearUserCommand();
    
    // set up regex to match commands with what we're looking for
    smatch matches;
    
    // this searches for words only (no numbers/white space)
    regex commandHunter("\\b[^\\d\\W]+\\b");
    
    // this searches for numbers only (it's how we identify ports)
    regex portHunter("\\b[0-9]{1,}");
    
    // save the userCommand string, because we're about to demolish it
    string passOne = userCommand;
    // string must be saved twice, because we'll be going over it twice
    string passTwo = userCommand;
    
    // first pass: check for word-only commands
    while(regex_search(passOne, matches, commandHunter)){
        // for all matches
        for(auto i:matches){
            // once a match is found, push it back
            this->parsedCommand.push_back(i);
            // trim the found match from the string being searched
            passOne=matches.suffix().str();
        }
    }
    
    // second pass: check for number-only commands (ports)
    while(regex_search(passTwo, matches, portHunter)){
        // for all matches
        for(auto i:matches){
            // once a match is found, push it to the port list (remember to convert to an int, so your program doesn't implode)
            this->portsToScan.push_back(stoi(i));
            // trim the found match from the string, and continue searching for matches
            passTwo=matches.suffix().str();
        }
    }
    
    // identify first argument
    // if the first argument is empty
    if(this->parsedCommand.empty()){
        // inform the user, clear the string, and let them try again
        cout << "You didn't enter a command. Try again, perhaps.";
        userCommand.clear();
        return;
    }
    // otherwise, decide if the primary command is for help, settings, or scanning
    else if(parsedCommand[0]=="help"){
        cout << "Asking for help?\n";
        return;
    }
    else if(parsedCommand[0]=="scan"){
        cout << "Scanning ... \n";
        return;
    }
    else if(parsedCommand[0]=="settings"){
        // open settings
        cout << "Let's open up our settings...\n";
        return;
    }
    else if(parsedCommand[0]=="exit"){
        cout << "Exiting ... \n";
        return;
    }
    else{
        cout << "Command not recognized. Try again.\n";
    }
    
    // confirm secondary arguments are well formed
    
    // run appropriate function based on argument results
    
//    // if potential help request
//    if(userCommand.find("help")!=string::npos){
//        // if found, send the command to HelpModule for processing
//        cout << "Asking for help?\n";
//    }
//    else if(userCommand.find("debug")!=string::npos){
//                cout << "debugging: \n";
//                debug(4767);
//    }
//    // if scan, call scanner
//    else{
//
//    }
    return;
}

void NoobScan::clearUserCommand(){
    this->parsedCommand.clear();
    this->portsToScan.clear();
    return;
}

void NoobScan::debug(int debugPort){
    cout << "testing debug\n";
    
    //TODO: add try catch block to this. Maybe isolate the ourScanner output before equating it to s_addr?
//1
//    struct sockaddr_in socketToScan;
//    socketToScan.sin_addr.s_addr = *(unsigned long*) ((ourScanner->returnHostIP("www.google.com"))->h_addr);
//    if(socketToScan.sin_addr.s_addr == NULL){
//        cout << "no ip address associated.\n";
//    }
//
//    else{
//        cout << inet_ntoa(socketToScan.sin_addr);
//    }
//2
//    if(ourScanner->returnHostIP("www.fhjkgshuilhusl.com")==NULL){
//        cout << "It's null\n";
//    }
//    else{
//        cout<<"Not null\n.";
//    }
//3
    //TODO: find out why this returns double prompt if it fails
    //ourTCPScan->runScan(debugPort);
//    vector<int> testVector{80,120,4767};
//    ourTCPScan->runMultiScan(testVector);
//    ourScanner->getHostMac();
//    ourScanner->debug();
    
            // parse the command string - regex used to
        //    sregex_iterator commandIterator(userCommand.begin(), userCommand.end(), commandHunter);
        //    sregex_iterator empty;
        //    while(commandIterator!=empty){
        //        matches=*commandIterator;
        //        parsedCommand.push_back(matches.str());
        //        commandIterator++;
        //    }
        
    //    sregex_iterator portIterator(userCommand.begin(), userCommand.end(), portHunter);
    //    while(portIterator!=empty){
    //        matches=*portIterator;
    //        //portsToScan.push_back(stoi(portIterator));
    //        portIterator++;
    //    }
      
    
    //    // output parsed for debug
    //    cout << "printing command split:\n";
    //    for(auto const & item : parsedCommand){
    //        cout << item << endl;
    //    }
    //
    //    cout << "printing port split:\n";
    //    for(auto const & item : portsToScan){
    //        cout << item << endl;
    //    }

    
}
