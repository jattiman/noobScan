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
    void printLogo();
    void welcomeText();
    void initialPrompt();
    void systemCheck();
    void opCheck();
    void adminCheck();
    string promptUser();
    void intakeCommands();
    void commandResponse(string userCommand);
    void inspectArgs(string userCommand);
    void parseUserArgument(string userCommand);
    NoobCodes reviewPrimaryCommand();
    NoobCodes reviewSecondaryCommands(NoobCodes commandType);
    void helpRequestCheck();
    void scanRequestCheck();
    void settingsRequestCheck();
    void clearUserCommand();
    void debug(int debugPort = 80);
    
private:
    HelpModule *ourHelper;
    Recorder *userRecorder;
    ScanAddress *ourScanner;
    TCPScanner *ourTCPScan;
    UDPScanner *ourUDPScan;
    SYNScanner *ourSYNScan;
    
    string ourCommand;
    string ourResult;
    bool isAdmin = false;
    //vector<int> portsToScan;
    vector<string> parsedCommand;
    vector<int> portsToScan;
    
};

#endif /* NoobScan_h */
