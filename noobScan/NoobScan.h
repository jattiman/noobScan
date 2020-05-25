/**/
/*
   main.cpp of noobScan, a novice port scanner
   main.cpp welcomes the user and runs scan options
*/
/**/

#ifndef NoobScan_h
#define NoobScan_h

#include "ScanAddress.h"
#include "Recorder.h"
#include "HelpModule.h"
#include "TCPScanner.h"
#include "UDPScanner.h"
#include "SYNScanner.h"
#include "NoobCodes.h"

// for socket address structures
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <iostream>
// for identifying and splitting strings
#include <regex>
#include <sstream>
// admin privileges check
#include <grp.h>
#include <pwd.h>

using namespace std;


class NoobScan{
public:
    NoobScan();
    
    // print the program text logo
    void printLogo();
    
    // welcome the user
    void welcomeText();
    
    // display the initial prompt
    void initialPrompt();
    
    // confirm the user has the proper system settings for all features of this program
    void systemCheck();
    
    // confirm user has the right operating system
    void opCheck();
    
    // confirm user is running admin privileges
    void adminCheck();
    
    // prompt user for their input
    string promptUser();
    
    // take in user commands
    void intakeCommands();
    
    // respond to the user command
    void commandResponse(string userCommand);
    
    // inspect user command arguments
    void inspectArgs(string userCommand);
    
    // parse user command arguments
    void parseUserArgument(string userCommand);
    
    // review first command
    NoobCodes reviewPrimaryCommand();
    
    // review sub (secondary) commands
    NoobCodes reviewSecondaryCommands(NoobCodes commandType);
    
    // respond to help requests
    void helpRequestCheck();
    
    // respond to scan requests
    void scanRequestCheck();
    
    // respond to settings requests
    void settingsRequestCheck();
    
    NoobCodes checkScanType();
    
    // clear commands for next round of questions
    void clearUserCommand();
    
    // debug (for testing purposes)
    void debug(int debugPort = 80);
    
private:
    
    HelpModule *ourHelper;
    Recorder *userRecorder;
    ScanAddress *ourScanner;
    TCPScanner *ourTCPScan;
    UDPScanner *ourUDPScan;
    SYNScanner *ourSYNScan;
    
    // string holding user command
    string ourCommand;
    
    // string holding result of an action
    string ourResult;
    
    // bool reflecting if user has admin rights to their system
    bool isAdmin = false;
    
    // vector holding the string-based user commands
    vector<string> parsedCommand;
    
    // vector holding port numbers to scan from user commands
    vector<int> portsToScan;
    
    // string holding the IP address from the user command
    string ipToScan;
    
};

#endif /* NoobScan_h */
