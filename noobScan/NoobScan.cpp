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
    
    // display the directions
    ourHelper->displayDirections();
    
    // act on user commands
    this->intakeCommands();
}

// intakes all commands from user - main function of this class
void NoobScan::intakeCommands(){
    //while(ourCommand.compare("exit")){
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
    smatch matches;
    //regex commandHunter("[a-zA-Z]{1,}");
    regex commandHunter("\\b[^\\d\\W]+\\b");
    regex portHunter("[\\d]{1,}");
    //regex portHunter("\\b[^\\W\\d]+\\b");
    
    vector<string> parsedCommand;
    vector<int> portsToScan;
    
    string passOne = userCommand;
    string passTwo = userCommand;
    
    // parse the command string - regex used to
//    sregex_iterator commandIterator(userCommand.begin(), userCommand.end(), commandHunter);
//    sregex_iterator empty;
//    while(commandIterator!=empty){
//        matches=*commandIterator;
//        parsedCommand.push_back(matches.str());
//        commandIterator++;
//    }
    
    while(regex_search(passOne, matches, commandHunter)){
        for(auto i:matches){
            //cout << " " << i << " ";
            parsedCommand.push_back(i);
            passOne=matches.suffix().str();
        }
    }
    
//    sregex_iterator portIterator(userCommand.begin(), userCommand.end(), portHunter);
//    while(portIterator!=empty){
//        matches=*portIterator;
//        //portsToScan.push_back(stoi(portIterator));
//        portIterator++;
//    }
  
    while(regex_search(passTwo, matches, portHunter)){
        for(auto i:matches){
            //cout << " " << i << " ";
            portsToScan.push_back(stoi(i));
            passTwo=matches.suffix().str();
        }
    }
    // output parsed for debug
    cout << "printing command split:\n";
    for(auto const & item : parsedCommand){
        cout << item << endl;
    }
    
    cout << "printing port split:\n";
    for(auto const & item : portsToScan){
        cout << item << endl;
    }
    
    // identify first argument
    
    // confirm secondary arguments are well formed
    
    // run appropriate function based on argument results
    
    // if potential help request
    if(userCommand.find("help")!=string::npos){
        // if found, send the command to HelpModule for processing
        cout << "Asking for help?\n";
    }
    else if(userCommand.find("debug")!=string::npos){
                cout << "debugging: \n";
                debug(4767);
    }
    // if scan, call scanner
    else{
        
    }
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
}
