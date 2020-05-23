//
//  NoobScan.cpp
//  noobScan
//
//  Created by John Atti on 4/9/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#include "NoobScan.h"

NoobScan::NoobScan(){
    // instantiate member objects
    this->ourHelper = new HelpModule();
    this->userRecorder = new Recorder();
    this->ourScanner = new ScanAddress();
    this->ourTCPScan = new TCPScanner();
    this->ourUDPScan = new UDPScanner();
    this->ourSYNScan = new SYNScanner();
    
    // clear any noise in out command strings
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
    cout << "Let's make sure you won't run into any trouble, ";
    
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
    cout << userInfo->pw_gecos << " (neat username).\n\n";
    
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
        cout <<"\tYou don't seem to be an admin.\n\tYou can try running commands, but you will run into trouble on some scans.\n\n\tStill, feel free to use this as a helper tool, to learn more about the wonderful world of port scanning.\n\n";
    }
}

void NoobScan::opCheck(){
    cout << "Checking operating system...\n\n";
    
    #if defined(__APPLE__)
        cout << "\tYou're using an OSX variant. That's what this program was made for, so you should be ok!\n\n";
    #elif defined(Macintosh)
        cout << "\tYou're using an older MacOS, but I think things will work.\n\tTread carefully!\n\n";
    #elif defined(_WIN32)
        cout << "\tWindows, huh? This program is not going to run well for you. Sorry.\n\n";
    #elif defined(__unix__)
        cout << "\tLooks like you're using a unix system, but not OSX.\n\tCertain scans may not work for you.\n\n";
    #elif defined(__gnu_linux__)
        cout << "\tLooks like you're using Linux.\n\tYou'll probably run into some problems executing scans, but feel free to use the help commands.\n\n";
    #else
        cout << "\tHaven't come across your OS in a while.\n\tYou might encounter some problems with this program.\n\n";
    #endif
    return;
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
    NoobCodes userRequest;
    
    // parse the argument into commands and ports
    this->parseUserArgument(userCommand);
    
    // identify first argument
    userRequest = this->reviewPrimaryCommand();
    
    if(userRequest==NoobCodes::fail){
        return;
    }
    
    // confirm secondary arguments are well formed
    this->reviewSecondaryCommands(userRequest);
    
    // run appropriate function based on argument results
    

    return;
}

void NoobScan::parseUserArgument(string userCommand){
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
    return;
}

NoobCodes NoobScan::reviewPrimaryCommand(){
    // if the first argument is empty
    if(this->parsedCommand.empty()){
        // inform the user and let them try again
        cout << "You didn't enter a command. Try again, perhaps?\n";
        return NoobCodes::fail;
    }
    
    // otherwise, decide if the primary command is for help, settings, or scanning (among other things). Thanks to the power of regex, we can compare exact matches without having to worry about extra spaces and the like.
    else if(parsedCommand[0].compare("help")==0){
        cout << "Asking for help?\n";
        return NoobCodes::helpRequest;
    }
    else if(parsedCommand[0].compare("scan")==0){
        cout << "Scanning ... \n";
        return NoobCodes::scanRequest;
    }
    else if(parsedCommand[0].compare("settings")==0){
        // open settings
        cout << "Let's open up our settings...\n";
        return NoobCodes::settingsRequest;
    }
    else if(parsedCommand[0].compare("debug")==0){
        // run super secret debug function
        cout << "Running debug function...\n";
        return NoobCodes::debugRequest;
    }
    else if(parsedCommand[0].compare("exit")==0){
        return NoobCodes::exitRequest;
    }
    else{
        cout << "Command not recognized. Try again.\n";
        return NoobCodes::fail;
    }
}

NoobCodes NoobScan::reviewSecondaryCommands(NoobCodes commandType){
    switch (commandType) {
        case NoobCodes::helpRequest:
            this->helpRequestCheck();
            break;
        case NoobCodes::scanRequest:
            this->scanRequestCheck();
            break;
        case NoobCodes::settingsRequest:
            this->settingsRequestCheck();
            break;
        case NoobCodes::debugRequest:
            this->debug();
            break;
        case NoobCodes::exitRequest:
            cout << "Exiting...\n";
            break;
        default:
            cout << "No command type.\n";
            break;
    }
    return NoobCodes::fail;
}

void NoobScan::helpRequestCheck(){
    size_t parsedCount=0;
    
    // count parsedCommand entries
    parsedCount=parsedCommand.size();
    
    switch (parsedCount) {
        // if there is only 1 item in the command
        case 1:
            // check to see if there are port numbers to define
            if(portsToScan.size()>0){
                this->ourHelper->returnInfo(portsToScan[0]);
            }
            // if no port numbers, give user general help info
            else{
                this->ourHelper->displayOptions();
            }
            break;
        case 2:
            this->ourHelper->returnInfo(parsedCommand[1]);
        default:
            break;
    }
    
    // if zero secondary commands, give general help text
    
    // if 1, call the help module to give definitions/help based on that term
    
    // if more than 1, inform the user that only the first argument is being used
    
    return;
}

void NoobScan::scanRequestCheck(){
    cout << "Scan request registered\n";
    // count parsedCommand entries
    
    // If total (including scan request) is more than 2
        // check the last command for special port selection
        // if 3 is a special and 2 is a specific scan type, send to scanner function
    
    // If total is 2
        // confirm scan type is well formed,
        // confirm that there are portsToScan (at least 1 port is entered)
        // and send scan to scanner function (tcp/udp)
    
    // If total is less than 2, the command is malformed
    // We COULD prompt the user to enter a scan type ...
    
    return;
}

void NoobScan::settingsRequestCheck(){
    cout << "Settings request registered\n";
    // count parsedCommand entries
    
    // if extra arguments present (3+), inform the user and only go on first additional argument
    
    // if portsToScan is not empty, inform the user it will be ignored/discarded
    
    // if no additional entries (1 arg), take to main settings menu
    
    // if additional entry (2 arg), check to see if it belongs to a sub-page on settings, and take the user there
    
    
    
    return;
}

void NoobScan::clearUserCommand(){
    this->parsedCommand.clear();
    this->portsToScan.clear();
    return;
}

// code I'm experimenting with, or have thrown away
void NoobScan::debug(int debugPort){
//    cout << "testing debug\n";
//    ourHelper->populateDirectory();
//    ourHelper->returnInfo("1592");
//    ourHelper->returnInfo("nondictionary term");
//    ourHelper->returnInfo("15number alpha");
    
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
    ourScanner->getHostMac();
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
    
}
