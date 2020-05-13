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

//#include "NoobCodes.h"

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
// for splitting strings
#include <regex>
#include <sstream>

using namespace std;


class NoobScan{
public:
    NoobScan();
    void printLogo();
    void welcomeText();
    void initialPrompt();
    string promptUser();
    void intakeCommands();
    void commandResponse(string userCommand);
    void inspectArgs(string userCommand);
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
    //vector<int> portsToScan;
    
};

#endif /* NoobScan_h */
