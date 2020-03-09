/**/
/*
   main.cpp of noobScan, a novice port scanner
   main.cpp welcomes the user and runs scan options
*/
/**/

// for socket address structures
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>


#include "Scanaddress.h"
#include "Recorder.h"
#include "HelpModule.h"


using namespace std;

string promptUser(){
    // welcome user
    cout << "Welcome to Noob Scan!\n";
    // prompt user for their command
    cout << "Type 'help' for directions, 'settings' to adjust settings, 'exit' to exit, or simply enter your scan command.\n";
    string userCommand;
    getline(cin, userCommand);
    return userCommand;
}


int main(int argc, const char * argv[]) {
    Recorder userRecorder;
    
    string ourCommand;
    // retrieve the command from the user
    ourCommand = promptUser();
    userRecorder.recordRequest(ourCommand);
    cout << "command was ";
    userRecorder.showHistory();
    
    // regex interpret ourCommand to see what the user mainly wants
    
    // If command asked for help
    
    // If command asked for settings
    
    // If command asked for scan
    
    // If command asked for exit
    
    
    return 0;
}
