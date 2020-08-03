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
#include <iomanip>
// for identifying and splitting strings
#include <regex>
#include <sstream>
// admin privileges check
#include <grp.h>
#include <pwd.h>
// string to unsigned conversion
#include <cstdlib>

using namespace std;


class NoobScan{
public:
    NoobScan();
    
    // confirm user input is valid
    int getValidInput(int minNum=0, int maxNum=65535);
    
    // output additional feedback
    void outputFeedback(string feedback);
    
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
    
    // confirm user host IP is retrievable
    void hostIPCheck();
    
    // prompt user for their input
    string promptUser(bool sign=true);
    
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
    
    NoobCodes findSettingsRequestType(string userRequest);
    
    NoobCodes displaySettings(NoobCodes settings=NoobCodes::settingsRequest);
    
    
    
    NoobCodes checkScanType();
    
    void setSystemFeedback(bool isOn);
    
    void setRoot(bool rootStatus);
    
    void setAdmin(bool adminStatus);
    
    bool getSystemFeedback();
    
    bool getIsRoot();
    
    bool getIsAdmin();
    
    // clear commands for next round of questions
    void clearUserCommand();
    
    void displayUserCommands();
    
    void displayUserPortRequests();
    
    
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
    
    // bool reflecting if user has root access on their system
    bool isRoot = false;
    
    // bool reflecting if user wants more program feedback
    bool systemFeedback = false;
    
    // vector holding the string-based user commands
    vector<string> parsedCommand;
    
    // vector holding port numbers to scan from user commands
    vector<unsigned> portsToScan;
    
    // string holding the IP address from the user command
    vector<string> ipToScan;
    
    // string holding the URL (if no IP) from the user command
    vector<string> siteToScan;
    
};

#endif /* NoobScan_h */
