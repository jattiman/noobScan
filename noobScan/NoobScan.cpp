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


// prompt user
string NoobScan::promptUser(){
    // prompt user for their command
    cout << ">: ";
    string userCommand;
    getline(cin, userCommand);
    return userCommand;
}

void NoobScan::intakeCommands(){
    while(ourCommand.compare("exit")){
        // prompt user for command
        ourCommand = promptUser();
        
        // act on the command
        commandResponse(ourCommand);
        
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

void NoobScan::commandResponse(string userCommand){
    // clear result string
    
    // intake command
    
    // categorize base request
    
    // if potential help request
    if(userCommand.find("help")!=string::npos){
        // if found, send the command to HelpModule for processing
        cout << "Asking for help?\n";
    }
    
    // if scan, call scanner
    else{
        
    }
    // update result string with answer to request
    
    return;
}
