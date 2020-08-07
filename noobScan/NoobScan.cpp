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

int NoobScan::getValidInput(int minNum, int maxNum){
    int userInput;
    cout << "#: ";
    while(!(cin >> userInput) || userInput < minNum || userInput > maxNum){
        cin.clear();
        cin.ignore(500,'\n');
        cout << "Please select a valid option." << endl;
    }
    return userInput;
}

void NoobScan::outputFeedback(string feedback){
    if(this->systemFeedback){
        cout << feedback << endl;
    }
    return;
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
    cout << "Before we get started, let's make sure you won't run into any trouble ... ";
    
    // confirm user has admin rights
    this->adminCheck();
    
    // check user operating system for compatibility
    this->opCheck();
    
    // confirm user IP is retrievable (will be useful for certain scans)
    this->hostIPCheck();
    
    // mentally prepare user
    cout << "Please choose your next commands carefully ... \n\n";
}

void NoobScan::adminCheck(){
    
    // store getuid to eventually determine root
    uid_t ourID = getuid();
    
    // create structure to pull host machine user account info
    struct passwd *userInfo = getpwuid(ourID);
    
    // output username/full name to user. If they're logged in as root, it should be obvious through this output ("root" and "system administrator" will likely appear unless their defaults were changed). Otherwise, it'll be whatever their default username is for non-root profiles.
    cout << userInfo->pw_name << " (or should we call you " << userInfo->pw_gecos << "?).\n\n";
    
    // see if they are in the admin group
    cout << "Checking admin/root privileges ... \n\n";
    
    // a pointer to the group structure will allow you to see if "admin" privileges are there
    struct group *adminCheck = getgrnam("admin");
    
    // rotate through group member list for user
    while(*adminCheck->gr_mem != NULL) {
        
        // if you find the admin code under our username, they are likely an admin. Break out of the loop.
        if (strcmp(userInfo->pw_name, *adminCheck->gr_mem) == 0) {
            setAdmin(true);
            break;
        }
        // otherwise, keep cycling through the list to see if admin credentials pop up
        adminCheck->gr_mem++;
    }
    
    // confirm root is updated
    if(ourID==0){
        setRoot(true);
    }
    
    if(this->getIsAdmin() && this->getIsRoot()){
        cout << "\tLooks like you have admin rights on this account, AND have root access.\n\tThat's a good thing.\n\n";
    }
    else if(this->getIsAdmin()){
        cout << "\tLooks like you have admin rights on this machine, but you don't have root access.\n\tTry running this program as root, or with sudo (recommended), to unlock all the perks.\n\n\tAs is, you may run into some trouble when attempting to run certain scans, although the helper tool can still be used as a good resource.\n\n";
    }
    else{
        cout <<"\tYou don't seem to be an admin.\n\tYou can try running commands, but you will run into trouble on some scans.\n\n\tStill, feel free to use this as a helper tool, to learn more about the wonderful world of port scanning.\n\n";
    }
}

// confirm user host IP is retrievable
void NoobScan::hostIPCheck(){
    cout << "Retrieving your IP address ... \n";
    
    string hostIP = ourScanner->getHostIP(ourScanner->getInterface());
    
    // if the user isn't using wireless
    if(hostIP.empty()){
        
        // see if their IP can be read through their wired port
        hostIP=ourScanner->getHostIP("eth0");
        if(hostIP.empty()){
            cout << "\tWe're having problems retrieving your IP address, which means the scans may have some issues.\n\tPerhaps you aren't connected to the internet, or perhaps we aren't trying the correct interface name.\n\tThink you know what interface you're connecting under? Try entering it in the settings. Typically we check for en0 and eth0.\n\n";
            
            // TODO: (stretch) prompt user to enter in additional IP interface
            
            return;
        }
        else{
            // change port to wired for future scans
            ourScanner->setInterface("eth0");
        }
    }
    
    // output the user's IP address
    cout << "\tYour IP address is: " << hostIP << "\n";
    cout << "\tRemember: your IP address may change, but you should still always keep it private.\n\n";
    return;
    
}

// check user operating system to ensure it's the right one for this program
void NoobScan::opCheck(){
    cout << "Checking operating system...\n\n";
    
    // if the user is using an OSX (macOS) variant, let them know they shouldn't have trouble running the program. Otherwise, let them know they'll run into issues, based on their OS.
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
        
        // if for some reason the command is empty (return issue), prompt again without the prompt notice.
        if(ourCommand.empty()){
            flush(cout);
            this->ourCommand = promptUser(false);
        }
        
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
        // output the answer
        //TODO: make this interpret the noobcodes...
        cout << this->ourResult;
        
    }
    return;
}

// prompt user
string NoobScan::promptUser(bool sign){
    // prompt user for their command
    if(sign){
        cout << ">: ";
    }
    string userCommand;
    getline(cin, userCommand);
    return userCommand;
}

// respond to user commands by initiating the appropriate functions
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

//TODO: give this a noobcodes
void NoobScan::inspectArgs(string userCommand){
    NoobCodes userRequest;
    
    // parse the argument into commands and ports
    this->parseUserArgument(userCommand);
    
    // identify first argument
    userRequest = this->reviewPrimaryCommand();
    
    // if first argument doesn't match, return a fail command
    if(userRequest==NoobCodes::fail){
        // TODO: update record with fail command here
        return;
    }
    
    // confirm secondary arguments are well formed, and act accordingly
    // TODO: change this function to have a return code, and use that to update the record with the appropriate command here
    this->reviewSecondaryCommands(userRequest);
    
    
    
    // run appropriate function based on argument results

    return;
}

void NoobScan::parseUserArgument(string userCommand){
    // TODO: remove formatted URLs from the string
    
    // ensure any previous commands are empty
    clearUserCommand();
    
    // set up regex to match commands with what we're looking for
    smatch matches;
    
    // this searches for words only (no numbers/white space)
    /* breakdown of Regex info, for those curious:
    \\b
     [^\\d\\W]+
     \\b
    */
    regex commandHunter("\\b[^\\d\\W]+\\b");
    
    // this searches for numbers only (it's how we identify ports)
    //regex portHunter("\\b[0-9]{1,}");
    /* breakdown of Regex info, for those curious:
     [^\\.] Ensure number isn't preceeded by a '.' to avoid IP confusion
     \\b[0-9]+ look for numbers 0-9 (as many in a row as you'd like) at the start
     \\b(?!\\.) Ensure the number doesn't end with a '.' to avoid IP confusion
     */
    regex portHunter("[^\\.]\\b[0-9]+\\b(?!\\.)");
    
    // This searches for our IP address, if it's there (if not, we assume a URL is being used)
    /* breakdown of Regex info, for those curious:
    R makes the regex more readable (raw string). Using it here to show variety. Note that we don't need to use double escape characters (\\) with this mode.
     (\d{1,3}\. look for any number (from 1-3 in a row) followed by a '.'
     \d{1,3}\. same
     \d{1,3}\. same
     \d{1,3}) look for any number (from 1-3 in a row), but without a
     '.' after. This closes the IP address in IPV4
    */
    regex ipHunter(R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
    
    // save the userCommand string, because we're about to demolish it with 3 passes
    string passOne = userCommand;
    string passTwo = userCommand;
    string passThree = userCommand;
    
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
    // bool holding whether or not an error was encountered
    bool firstError=true;
    while(regex_search(passTwo, matches, portHunter)){
        // for all matches
        
        for(auto i:matches){
            // once a match is found, push it to the port list (remember to convert to number, so the program doesn't implode)
            try {
                // TODO: make check for good unsigned conversion
                // convert the string to an unsigned number
                unsigned long ourPort = stoul(i);
                portsToScan.push_back((unsigned int)ourPort);
            } catch (const std::invalid_argument) {
                if(firstError){
                    portsToScan.pop_back();
                    firstError=false;
                }
                cout << "Your formatting is off (misreading for " << i << "). Results may be unexpected.\n";
            }
            // trim the found match from the string, and continue searching for matches
            passTwo=matches.suffix().str();
        }
    }
    
    // third pass: check for IP addresses
    while(regex_search(passThree, matches, ipHunter)){
        // for all matches
        for(auto i:matches){
            // add in the IP address as the next command
            parsedCommand.push_back(i);
            ipToScan.push_back(i);
            passThree=matches.suffix().str();
        }
    }
    
}

NoobCodes NoobScan::reviewPrimaryCommand(){
    
    // if feedback is on, print user commands and ports selected
    if(this->systemFeedback){
        this->displayUserCommands();
        this->displayUserPortRequests();
    }
    
    // if the first argument is empty
    if(this->parsedCommand.empty()){
        // inform the user and let them try again
        cout << "You didn't enter a command. Try again, perhaps?\n";
        return NoobCodes::fail;
    }
    
    // otherwise, decide if the primary command is for help, settings, scanning, etc, and act accordingly.
    else if(parsedCommand[0].compare("help")==0){
        outputFeedback("Asking for help?\n");
        return NoobCodes::helpRequest;
    }
    else if(parsedCommand[0].compare("scan")==0){
        outputFeedback("Scanning ... \n");
        return NoobCodes::scanRequest;
    }
    else if(parsedCommand[0].compare("settings")==0){
        // open settings
        outputFeedback("Let's open up our settings...\n");
        return NoobCodes::settingsRequest;
    }
    else if(parsedCommand[0].compare("debug")==0){
        // run super secret debug function
        outputFeedback("Running debug function...\n");
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
        // if there are 0 non-port items in the command
        case 1:
            // check to see if there are port numbers to define
            // if multiple
            if(portsToScan.size()>1){
                // display warning, and define only the first
                this->ourHelper->helpWarning();
                this->ourHelper->returnInfo(portsToScan[0]);
            }
            // if only 1 port, define that port
            else if(portsToScan.size()>0){
                this->ourHelper->returnInfo(portsToScan[0]);
            }
            // if no port numbers, give user general help info
            else{
                this->ourHelper->displayOptions();
            }
            break;
        // if 1 non-port item is present, return info on the parsed command
        case 2:
            // if there are also ports requested, display warning
            if(portsToScan.size()>0){
                this->ourHelper->helpWarning();
            }
            // define the non-port item requested
            this->ourHelper->returnInfo(parsedCommand[1]);
            break;
        case 3:
        case 4:
        case 5:
        case 6:
            this->ourHelper->helpWarning();
            this->ourHelper->returnInfo(parsedCommand[1]);
            break;
        default:
            break;
    }
    
    return;
}



void NoobScan::scanRequestCheck(){
    cout << "Scan request registered\n";
    
    // holder for scan type
    NoobCodes scanType;
    
    // count parsedCommand entries
    size_t parsedCount=0;
    parsedCount=parsedCommand.size();
    
    string scanTarget;
    
    // if at least 3 arguments (scan [scan type] [destination]), the command is most likely sufficiently formed. Remember: ports are not part of the parsedCommand
    if(parsedCount >= 3){
        // check scan type
        scanType=checkScanType();
        
        // confirm sub arguments called correctly
        //TODO: pull URL out and turn it into an IP address
        
        // run scan
        if(scanType==NoobCodes::tcp){
            this->ourTCPScan->runMultiScan(portsToScan,ipToScan[0]);
        }
        else if(scanType==NoobCodes::udp){
            //this->ourUDPScan->runMultiScan(portsToScan,ipToScan[0]);
            this->ourUDPScan->runScan(portsToScan[0], getIsRoot());
        }
        else{
            cout << "Scan type currently unavailable.\n";
        }
        
    }
    // if less than 3 arguments, the command is malformed
    else{
        cout << "It looks like you might be having some trouble formulating your scan. Please follow the scan directions:" << endl;
        this->ourHelper->returnInfo("scan");
    }
    
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

// processes scan type and acts accordingly
NoobCodes NoobScan::checkScanType(){
    string ourScanType = parsedCommand[1];
    if(ourScanType=="tcp"){
        outputFeedback("You're requesting a TCP scan\n");
        return NoobCodes::tcp;
    }
    else if(ourScanType=="udp"){
        outputFeedback("You're requesting a UDP scan\n");
        return NoobCodes::udp;
    }
    else if(ourScanType=="syn"){
        outputFeedback("You're requesting a SYN scan\n");
        return NoobCodes::syn;
    }
    else if(ourScanType=="fin"){
        outputFeedback("You're requesting a FIN scan\n");
        return NoobCodes::fin;
    }
    else{
        cout << "Something went wrong ... \n";
        return NoobCodes::fail;
    }
}

void NoobScan::settingsRequestCheck(){
    outputFeedback("Settings request registered\n");
    
    // NoobCodes return for settings response
    // NoobCodes settingsHolder;
    
    // count parsedCommand entries
    size_t parsedCount=0;
    parsedCount=parsedCommand.size();
    
    // if user is requesting settings for a port
    if(portsToScan.size()!=0){
        // Inform them they have formatted their request incorrectly
        cout << "Port numbers do not apply to settings.\n\n";
        
        // dump to general settings menu.
        displaySettings();
    }
    // if user is requesting a specific setting to toggle
    else if(parsedCount==2){
        // let the user know the setting they're requesting info for.
        string debugString = "You're asking specifically to access the setting for: " + parsedCommand[1];
        outputFeedback(debugString);
        cout << endl;
        
        this->displaySettings(findSettingsRequestType(parsedCommand[1]));
        // check if user's command corresponds to a setting, and react appropriately
    }
    // if user just entered "settings"
    else if(parsedCount==1){
        // general settings request - show all settings menu
        cout << "You're asking for general settings.\n\n";
        this->displaySettings(NoobCodes::settingsRequest);
    }
    else{
        // TODO: if additional entry (2 arg), check to see if it belongs to a sub-page on settings, and take the user there

        
        // incorrectly formed settings request
        cout << "Your settings request was formatted incorrectly. Either input \"settings\", or \"settings [specific setting topic]\"\n\n";
        // dump them into the general settings menu
    }
    
    return;
}

// interprets userRequest to display appropriate settings menu
NoobCodes NoobScan::findSettingsRequestType(string userRequest){
    if(userRequest=="delay"){
        return NoobCodes::settingsForDelay;
    }
    else if(userRequest=="groups"){
        return NoobCodes::settingsForScanGroups;
    }
    else if(userRequest=="dictionary"){
        return NoobCodes::settingsForDictionary;
    }
    else if(userRequest=="recorder"){
        return NoobCodes::settingsForRecorder;
    }
    else if(userRequest=="timeout"){
        return NoobCodes::settingsForTimeouts;
    }
    else if(userRequest=="feedback"){
        return NoobCodes::settingsForDebugText;
    }
    else if(userRequest=="scan"){
        return NoobCodes::settingsForAssistedScan;
    }
    else{
        return NoobCodes::fail;
    }
}

void NoobScan::settingsOptions(int & userAnswer, NoobCodes & settings){
    // display default menu and get input
    cout << "\nPlease select from the following options:\n"
    << "\t1. Assisted Scan\n"
    << "\t2. Dictionary\n"
    << "\t3. Recorder Settings\n"
    << "\t4. Scan groups\n"
    << "\t5. Scan delay time\n"
    << "\t6. Scan timeout time\n"
    << "\t7. Toggle feedback text\n"
    << "\t8. Exit settings\n\n";
    userAnswer=getValidInput(1,8);
    switch (userAnswer) {
        case 1:
            settings=NoobCodes::settingsForAssistedScan;
            break;
        case 2:
            settings=NoobCodes::settingsForDictionary;
            break;
        case 3:
            settings=NoobCodes::settingsForRecorder;
            break;
        case 4:
            settings=NoobCodes::settingsForScanGroups;
            break;
        case 5:
            settings=NoobCodes::settingsForDelay;
            break;
        case 6:
            settings=NoobCodes::settingsForTimeouts;
            break;
        case 7:
            settings=NoobCodes::settingsForDebugText;
            break;
        default:
            settings=NoobCodes::exitRequest;
    }

    return;
}

void NoobScan::settingsDictionary(int & userAnswer, NoobCodes & settings){
    cout << "Would you like to add to the dictionary, or view an entry?\n"
        << "\t1. add\n"
        << "\t2. view an entry\n"
        << "\t3. neither\n";
    userAnswer = getValidInput(1,3);
    if(userAnswer==1){
        this->ourHelper->addToDictionary();
        settings = NoobCodes::restart;
    }
    else if(userAnswer==2){
        
        // clear the input buffer to help prevent funny business
        cin.ignore(256,'\n');
        string viewEntry;
        
        // prompt user and attempt to retrieve entry
        cout << "Please enter the term you'd like to look up: ";
        getline(cin,viewEntry);
        this->ourHelper->returnInfo(viewEntry);
        settings = NoobCodes::restart;
    }
    else{
        settings = NoobCodes::restart;
    }
    
    return;
}

void NoobScan::settingsRecorder(int & userAnswer, NoobCodes & settings){
    // display recorder on/off status
    cout << "The recorder is currently ";
    if(this->userRecorder->getRecorderStatus()){
        cout << "on.\n\n";
    }
    else{
        cout << "off.\n\n";
    }
    
    // prompt user to modify recorder
    cout << "\t1. Turn recorder on.\n"
    << "\t2. Turn recorder off.\n"
    << "\t3. Exit (keep current recorder setting)\n";
    userAnswer = getValidInput(1,3);
    
    // react accordingly
    if(userAnswer==1){
        this->userRecorder->setRecorderStatus(true);
        cout << "\tRecorder: on.\n";
        settings = NoobCodes::restart;
    }
    else if(userAnswer==2){
        this->userRecorder->setRecorderStatus(false);
        cout << "\tRecorder: off.\n";
        settings = NoobCodes::restart;
    }
    else{
        cout << "\tExiting recorder settings.\n";
        settings = NoobCodes::restart;
    }
    
    return;
}

// displays the timeouts options
void NoobScan::settingsTimeouts(int & userAnswer, NoobCodes & settings){
    cout << "The current timeout limit for server response is: " << this->ourScanner->getSleepTimer() << " seconds.\n";
    cout << "Please enter your new time (between 0 and 10 seconds): " << endl;
    
    unsigned int newTime = getValidInput(0,10);
    
    this->ourScanner->setSleepTimer(newTime);
    
    cout << "The new timeout limit for server response is: " << this->ourScanner->getSleepTimer() << " seconds.\n";
    
    settings = NoobCodes::restart;
    
    return;
}

// displays the debug options
void NoobScan::settingsDebug(int & userAnswer, NoobCodes & settings){
    // display debug text on/off status
    cout << "Feedback text is currently ";
    if(this->getSystemFeedback()){
        cout << "on.\n\n";
    }
    else{
        cout << "off.\n\n";
    }
    
    // give user options
    cout << "\t1. Turn feedback on.\n"
    << "\t2. Turn feedback off.\n"
    << "\t3. Exit (keep current feedback setting)\n";
    userAnswer = getValidInput(1,3);
    
    // act accordingly
    if(userAnswer==1){
        this->setSystemFeedback(true);
        cout << "\tFeedback: on.\n";
        settings = NoobCodes::restart;
    }
    else if(userAnswer==2){
        this->setSystemFeedback(false);
        cout << "\tFeedback: off.\n";
        settings = NoobCodes::restart;
    }
    else{
        cout << "\tExiting feedback settings.\n";
        settings = NoobCodes::restart;
    }
    return;
}

// split input to strings
void NoobScan::splitString(string originString, vector<string> & splitString){
    
    // set create a holder for the pieces of the string
    string stringToken;
    
    // stringstream to parse the string
    stringstream ourStream(originString);
    
    // move each individual entry, with whitespace as a delimiter, into the referenced string vector
    while(getline(ourStream, stringToken, ' ')){
        splitString.push_back(stringToken);
    }
    
    return;
}

// pull all numbers from a string, and place them into a vector
void NoobScan::getNums(vector<unsigned> & ourNums){
    // set up variables to capture numbers from string
    vector<string> splitString;
    unsigned portHolder;
    string ourString;
    smatch matches;
    regex numberHunter("(\\d+)");
    bool tryAgain = false;
    
    do{
        if(tryAgain){
            cout << "Please try entering your port numbers again: ";
        }
        ourNums.clear();
        ourString.clear();
        splitString.clear();
        
        // intake the port numbers
        cin.ignore();
        getline(cin,ourString);
        
        // split the string
        this->splitString(ourString, splitString);
        
        // take in only the entries that are full numbers
        for(auto & splitElement: splitString){
            if(regex_match(splitElement, numberHunter)){
                portHolder=(unsigned)stoul(splitElement);
                ourNums.push_back(portHolder);
            }
        }
        
        // have user confirm they entered the correct ports
        cout << "\nAre you sure you want to add these ports?: ";
        for(auto & aNumber: ourNums){
            cout << aNumber << " ";
        }
        cout << endl;
        cout << "\t1. yes\n";
        cout << "\t2. no\n";
        
        // use a ternary operator to show that I can!
        tryAgain = (this->getValidInput(1,2)==1)? false : true;
        
    }while(tryAgain);
    
    ourScanner->setCustomList(ourNums);
    
    return;
}

// displays scan groups options
void NoobScan::settingsGroups(int & userAnswer, NoobCodes & settings){

    // display scan groups
    cout << "Select your scan group to learn more:\n";
    cout << "\t1. Chat\n";
    cout << "\t2. Gaming\n";
    cout << "\t3. Malicious\n";
    cout << "\t4. Peer\n";
    cout << "\t5. Popular \n";
    cout << "\t6. Streaming\n";
    cout << "\t7. Make your own.\n";
    cout << "\t8. Exit\n";
    
    // allow edit of scan groups
    userAnswer=getValidInput(1,8);
    
    switch (userAnswer) {
        case 1:
            this->ourHelper->returnInfo("chat");
            break;
        case 2:
            this->ourHelper->returnInfo("gaming");
            break;
        case 3:
            this->ourHelper->returnInfo("malicious");
            break;
        case 4:
            this->ourHelper->returnInfo("peer");
            break;
        case 5:
            this->ourHelper->returnInfo("popular");
            break;
        case 6:
            this->ourHelper->returnInfo("streaming");
            break;
        case 7:
            cout << "Please enter the ports you want to add to your own group (press enter when done): ";
            getNums(this->portsToScan);
            break;
        case 8:
            cout << "Exiting ... \n";
            settings = NoobCodes::restart;
            break;
        default:
            break;
    }
    
    return;
}

NoobCodes NoobScan::displaySettings(NoobCodes settings){
    
    int userAnswer = 0;
    while(settings != NoobCodes::exitRequest){
        
        // if a general settings request came in
        if(settings == NoobCodes::settingsRequest || settings == NoobCodes::restart){
            
            // display default menu and get input
            settingsOptions(userAnswer, settings);
            
            // restart the settings menu with the new setting in place
            continue;
        }
        // if the user wants to adjust the dictionary
        else if(settings == NoobCodes::settingsForDictionary){
            
            // send user to adjust/view the dictionary
            settingsDictionary(userAnswer, settings);
            
            // restart from the top of the settings menu
            continue;
        }
        // if the user wants to change the delay between scanning ports
        else if(settings == NoobCodes::settingsForDelay){
            
            // display delay time options (variable, or enter specific number)
            cout << "Your current delay time between ports is " << this->ourScanner->getSleepTimer() << endl << endl;
            cout << "\t1. Set specific delay time\n"
            << "\t2. Enable variable delay time between scans.\n"
            << "\t3. Exit (keep current delay setting)\n";
            userAnswer = getValidInput(1,3);
            // TODO: follow up with 1-3 answers
            
            continue;
        }
        else if(settings == NoobCodes::settingsForRecorder){
            
            // allow user to adjust recorder
            settingsRecorder(userAnswer, settings);
            
            // jump to the top of the menu
            continue;
        }
        else if(settings == NoobCodes::settingsForTimeouts){
            
            // allow user to adjust settings
            settingsTimeouts(userAnswer, settings);
            
            // jump to the top of the menu
            continue;
        }
        else if(settings == NoobCodes::settingsForDebugText){
            
            // allow user to turn debug text on/off
            settingsDebug(userAnswer, settings);
            
            // jump to top of menu
            continue;
        }
        else if(settings == NoobCodes::settingsForScanGroups){
            
            // take them to the settings page.
            settingsGroups(userAnswer, settings);
            
            // exit
            continue;
        }
        else if(settings == NoobCodes::settingsForAssistedScan){
            // TODO: create assisted scan system for user, where they enter the scan line by line. in new function: "what type of scan?" "scanning IP address or website?" "what IP address/website are you scanning?" "do you want to scan certain ports, or a port group?"
            cout << "Let's walk you through a scan ... \n";
            // send to ScanAddress slow scan function
            break;
        }
        else if(settings == NoobCodes::exitRequest){
            cout << "Exiting settings ... \n";
            settings = NoobCodes::exitRequest;
            break;
        }
        else{
            //TODO: look into iomanip for formatting here
            cout << "Invalid settings choice. Please select from the following:\n"
            << setw(11) << left << "\tdelay" << "\t-\tset delay between ports to scan\n"
            << setw(11) << left << "\tdictionary" << "\t-\tadd terms to dictionary\n"
            << setw(11) << left << "\tfeedback" << "\t-\toutput additional text to console during command response\n"
            << setw(11) << left << "\tgroups" << "\t-\tview and change scan groups\n"
            << setw(11) << left << "\trecorder" << "\t-\tenable or disable the recorder\n"
            << setw(11) << left << "\tscan" << "\t-\tcreate an assisted scan\n"
            << setw(11) << left << "\ttimeout" << "\t-\tset timeout length when waiting for server to respond\n" << endl;
            break;
        }
    }
    return NoobCodes::restart;
}

// turn on and off feedback
void NoobScan::setSystemFeedback(bool isOn){
    this->systemFeedback=isOn;
}

// turn on and off root indicator
void NoobScan::setRoot(bool rootStatus){
    this->isRoot=rootStatus;
}

// turn on and off admin indicator
void NoobScan::setAdmin(bool adminStatus){
    this->isAdmin=adminStatus;
}


// check if feedback is on
bool NoobScan::getSystemFeedback(){
    return this->systemFeedback;
}

bool NoobScan::getIsRoot(){
    return this->isRoot;
}

bool NoobScan::getIsAdmin(){
    return this->isAdmin;
}

// clears the user command
void NoobScan::clearUserCommand(){
    this->parsedCommand.clear();
    this->portsToScan.clear();
    return;
}

void NoobScan::displayUserCommands(){
    cout << "\tCommands input:\t";
    for(const auto & i: this->parsedCommand){
        cout << i << " ";
    }
    cout << endl;
    return;
}

void NoobScan::displayUserPortRequests(){
    cout << "\tPorts requested:\t";
    if(portsToScan.empty()){
        cout << "none.";
    }
    else{
        for(const auto & i: this->portsToScan){
            cout << i << " ";
        }
    }
    cout << endl;
    return;
}

// code I'm experimenting with, or have thrown away
void NoobScan::debug(int debugPort){
    
    userRecorder->showHistory();
    
    // other porthunter option
//    regex rx(R"((?:^|\s)([+-]?[[:digit:]]+(?:\.[[:digit:]]+)?)(?=$|\s))");


/*
    // get IP from URL
//    string ourString = ourScanner->getTargetIP("Scanme.Nmap.Org");
//    cout << "-" << ourString << "-" << endl;
    
    // get host machine IPs
    string test;
    test = ourScanner->getHostIP();
    cout << test << endl;
    
    test.clear();
    
    // get host machine's MAC
    test = ourScanner->getHostMac();
    cout << test << endl;
    
    // alt get host machine MAC
    ourScanner->debug();
*/
}
