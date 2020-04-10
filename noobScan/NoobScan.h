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

using namespace std;


class NoobScan{
public:
    NoobScan();
    void printLogo();
    string promptUser();
    
private:
    HelpModule *ourHelper;
    Recorder *userRecorder;
    ScanAddress *ourScanner;
    string ourCommand;
};

#endif /* NoobScan_h */
