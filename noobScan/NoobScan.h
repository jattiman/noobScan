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
    NoobCodes commandResponse(string userCommand, NoobCodes & category);
    
    // inspect user command arguments
    NoobCodes inspectArgs(string userCommand, NoobCodes & category);
    
    // parse user command arguments
    NoobCodes parseUserArgument(string userCommand);
    
    // review first command
    NoobCodes reviewPrimaryCommand();
    
    // review sub (secondary) commands
    NoobCodes reviewSecondaryCommands(NoobCodes commandType);
    
    // report errors to user screen
    void reportError(NoobCodes programError);
    
    // confirm IP for a website, if possible
    NoobCodes IPRequestCheck();
    
    // respond to help requests
    NoobCodes helpRequestCheck();
    
    // respond to scan requests
    NoobCodes scanRequestCheck();
    
    // respond to settings requests
    NoobCodes settingsRequestCheck();
    
    // interprets userRequest to display appropriate settings menu
    NoobCodes findSettingsRequestType(string userRequest);
    
    // the settings hub goes here
    NoobCodes displaySettings(NoobCodes settings=NoobCodes::settingsRequest);
    
    // displays the general settings options menu
    void settingsOptions(int & userAnswer, NoobCodes & settings);
    
    // directs user to run an assisted scan
    void settingsAssisted(int & userAnswer, NoobCodes & settings);
    
    // displays the dictionary options menu
    void settingsDictionary(int & userAnswer, NoobCodes & settings);
    
    // displays the delay options menu
    void settingsDelay(int & userAnswer, NoobCodes & settings);
    
    // displays the recorder options
    void settingsRecorder(int & userAnswer, NoobCodes & settings);
    
    // displays the timeouts options
    void settingsTimeouts(int & userAnswer, NoobCodes & settings);
    
    // displays the debug options
    void settingsDebug(int & userAnswer, NoobCodes & settings);
    
    // displays scan groups options
    void settingsGroups(int & userAnswer, NoobCodes & settings);
    
    // get numbers only from input
    void getNums(vector<unsigned> & ourNums);
    
    // split input to strings
    void splitString(string originString, vector<string> & splitString);
    
    // confirms which type of scan is being requested
    NoobCodes checkScanType();
    
    // sets system feedback
    void setSystemFeedback(bool isOn);
    
    // sets if user is root
    void setRoot(bool rootStatus);
    
    // sets if user is admin
    void setAdmin(bool adminStatus);
    
    // sets variable scan timing indicator (on/off)
    void setVariableScan(bool isOn);
    
    // returns variable scan time indicator
    bool getVariableScanStatus();
    
    // returns system feedback status
    bool getSystemFeedback();
    
    // returns root access
    bool getIsRoot();
    
    // returns user admin status
    bool getIsAdmin();
    
    // clear commands for next round of questions
    void clearCommandVectors();
    
    // displays user commands (used with user feedback to show the user how the machine interprets what they entered)
    void displayUserCommands();
    
    // displays the ports the user requested for scanning (used when user feedback is on)
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
    bool systemFeedback = true;
    
    // bool reflecting if user wants to turn on variable scan time
    bool variableTime = false;
    
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
