/**/
/*
 NoobScan.cpp of NoobScan, a novice portscanner
 Author: John Atti
 Finished 8/16/2020
*/
/**/


#include "NoobScan.h"

/**/
/*
NoobScan::NoobScan() NoobScan::NoobScan()
 
NAME
        NoobScan::NoobScan - class constructor
SYNOPSIS
        NoobScan::NoobScan()
DESCRIPTION
        Instantiates the NoobScan object with default variables.
RETURNS
        NoobScan class object.

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobScan::NoobScan(){
    // instantiate member objects where appropriate
    this->ourHelper = new HelpModule();
    this->userRecorder = new Recorder();
    this->ourScanner = new ScanAddress();
    
    // clear any noise in out command strings
    this->ourResult.clear();
    this->ourCommand.clear();
}
/*NoobScan();*/

/**/
/*
NoobScan::getValidInput() ScanAddress::getValidInput()

NAME
        NoobScan::getValidInput - confirm user input is valid
SYNOPSIS
        int NoobScan::getValidInput(int minNum, int maxNum)
            int minNum --> minimum acceptable number
            int maxNum --> maximum acceptable number
DESCRIPTION
        Function prompts the user for a integer input, handles improperly formatted input, and ensure the integer accepted is sufficient for the prompt given to the user elsewhere in the program. User will be continually prompted until a valid input is entered.
        Note: you will see that this function is in place throughout additional class structures as well. When I work on this program post-graduation, I intend on simplifying this, although I am resolved not to make it a global function.
RETURNS
        Integer with confirmed, appropriate user choice.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*int getValidInput(int minNum=0, int maxNum=65535);*/

/**/
/*
NoobScan::outputFeedback() NoobScan::outputFeedback()

NAME
        NoobScan::outputFeedback - outputs user feedback
SYNOPSIS
        void NoobScan::outputFeedback(string feedback);
            feedback --> string holding user feedback
DESCRIPTION
        This function checks to ensure that the user has toggled feedback on. If so, it will output the string to the screen. This saves a few lines of code setting up if statements elsewhere in the program.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::outputFeedback(string feedback){
    if(this->systemFeedback){
        cout << feedback << endl;
    }
    return;
}
/*void outputFeedback(string feedback);*/

/**/
/*
NoobScan::printLogo() NoobScan::printLogo()

NAME
        NoobScan::printLogo - prints the program logo
SYNOPSIS
        void NoobScan::printLogo();
DESCRIPTION
        This local was made possible by the folks at http://patorjk.com/software/taag/#p=display&f=Ogre&t=NoobScan. I had to tinker with a few of the spacing pieces to make it display appropriately when written in C++, but overall I think the logo makes it look fun, and is a good introduction to the program.
RETURNS
        Void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// output logo
//
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
/*void printLogo();*/

/**/
/*
NoobScan::welcomeText() NoobScan::welcomeText()

NAME
        NoobScan::welcomeText - welcomes the user to NoobScan.
SYNOPSIS
        void NoobScan::welcomeText();
DESCRIPTION
        Outputs welcome text to the user.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// give user the welcome text
void NoobScan::welcomeText(){
    cout << "Welcome to NoobScan, the newbie port scanner!\n"
    << "Please scan responsibly, and be ready to learn a thing or two.\n\n";
    return;
}
/*void welcomeText();*/

/**/
/*
NoobScan::initialPrompt() NoobScan::initialPrompt()

NAME
        NoobScan::initialPrompt - initial user prompt comes through here, which also outputs the user's status on their machine
SYNOPSIS
        void NoobScan::initialPrompt()
DESCRIPTION
        This function displays all opening information to the user, utilizing separate functions in the class. It is my attempt at trying to minimize the amount of code in any function.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void initialPrompt();*/

/**/
/*
NoobScan::systemCheck() NoobScan::systemCheck()

NAME
        NoobScan::systemCheck - checks and helps output system stats (admin/OS/IP/etc)
SYNOPSIS
        void NoobScan::systemCheck()
DESCRIPTION
        This function checks and helps output system stats for the user by calling separate functions.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::systemCheck(){
    cout << "Before we get started, let's make sure you won't run into any trouble ... ";
    
    // confirm user has admin/root rights
    this->adminCheck();
    
    // check user operating system for compatibility
    this->opCheck();
    
    // confirm user IP is retrievable (will be useful for certain scans)
    this->hostIPCheck();
    
    // mentally prepare user
    cout << "Please choose your next commands carefully ... \n\n";
}
/*void systemCheck();*/

/**/
/*
NoobScan::opCheck() NoobScan::opCheck()

NAME
        NoobScan::opCheck - checks user OS and responds accordingly
SYNOPSIS
        void NoobScan::opCheck()
DESCRIPTION
        This function will review the user's operating system to ensure it is running something compatible. As this program is only made for macOS systems, we simply warn any user brave enough to try running it on a non macOS machine that it may not behave appropriately. We check the OS with system macros.
RETURNS
        Void - no return value.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void opCheck();*/

/**/
/*
NoobScan::adminCheck() NoobScan::adminCheck()

NAME
        NoobScan::adminCheck - checks admin/root privileges
SYNOPSIS
        void NoobScan::adminCheck()
DESCRIPTION
        This function will check the user's administrator and root privileges, according to macOS environments. Some of these functions might work on other unix/linux variants, but it is not meant to.
        Function will rely on the passwd and group structures to get the ID information for the user's system.
        After it completes its check and updates the root and admin status of the class, it will output the information to the user so they know what their privilege level is.
        As an added fun bonus, it also pulls the user's login and user name, to greet them during this process. If they are root, it would be generic. Otherwise, it should be the name/username that they set for themselves.
RETURNS
        Void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void adminCheck();*/

/**/
/*
NoobScan::hostIPCheck() NoobScan::hostIPCheck()

NAME
        NoobScan::hostIPCheck - confirm user host IP is retrievable
SYNOPSIS
        void NoobScan::hostIPCheck();
DESCRIPTION
        This looks to see if the host IP address is able to be pulled. It checks through the default interface name ('en0'), and if not found, checks another common name ('eth0') before informing the user that the IP can't be found.
        If the IP is found on another port interface, the scanner's IP address adjusts based on this as the default interface, which will be used for future lookups.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void hostIPCheck();*/

/**/
/*
NoobScan::promptUser() NoobScan::promptUser()

NAME
        NoobScan::promptUser - prompt user for input
SYNOPSIS
        string NoobScan::promptUser(bool sign);
            sign --> bool telling function to output prompt indicator to screen
DESCRIPTION
        This is an interesting function. I was running into issues with user input based on stream buffering, and created this function, along with a secondary check function, to purify the input in the event that the user input was not being read by the system correctly. Saved me a big headache, and still not sure how to otherwise handle the i/o issue without these functions.
RETURNS
        String indicating what the user's command is.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*string promptUser(bool sign=true);*/

/**/
/*
NoobScan::intakeCommands() NoobScan::intakeCommands()

NAME
        NoobScan::intakeCommands - intakes all commands from user - main function of this class
SYNOPSIS
        void NoobScan::intakeCommands();
DESCRIPTION
        This is the main loop of the class, and therefore of the program. It will prompt the user for their command, ensure it is received correctly (no i/o errors), and then begin to act on the command.
        It is also in charge of ensuring that the recorder, when turned on, records the information appropriately.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::intakeCommands(){
    
    NoobCodes category;
    NoobCodes resultCode;
    while(true){
        
        // ensure our command string is clear to avoid string/buffer issues
        this->ourCommand.clear();

        // prompt user for command
        this->ourCommand = promptUser();
        
        // if for some reason the command is empty (return issue), prompt again without the prompt notice.
        if(this->ourCommand.empty()){
            flush(cout);
            this->ourCommand = promptUser(false);
        }
        
        // act on the command
        resultCode = this->commandResponse(ourCommand, category);
        
        if(ourCommand.compare("exit")==0){
            return;
        }
        
        // if Recorder is on
        if(userRecorder->getRecorderStatus()){
            
            // categorize the request and result
            userRecorder->categorizeOutcome(this->ourCommand, ourScanner->translateNoob(resultCode), ourScanner->translateNoob(category));
        }
        
        // clear user command
        this->ourCommand.clear();
    }
    return;
}
/*void intakeCommands();*/

/**/
/*
NoobScan::commandResponse() NoobScan::commandResponse()

NAME
        NoobScan::commandResponse - respond to user commands by initiating the appropriate functions
SYNOPSIS
        NoobCodes NoobScan::commandResponse(string userCommand, NoobCodes & category);
            userCommand --> string holding command from user
            category    --> enum holding the category being updated based on the command category, once categorized
DESCRIPTION
        This function will clear the command buffer, intake the user command, then call the appropriate function to inspect/act on it. That function wil return the outcome (good or failing command), and also update the category of the function.
RETURNS
        NoobCodes enum indicating if the command succeeded in being processed without issue.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::commandResponse(string userCommand, NoobCodes & category){
    
    // create a code to hold the final outcome result of the request
    NoobCodes finalOutcome = NoobCodes::success;
    
    // clear result string
    this->ourResult.clear();
    
    // intake command
    string commandManip = userCommand;
    
    // categorize base request
    finalOutcome = inspectArgs(commandManip, category);
    
    return finalOutcome;
}
/*NoobCodes commandResponse(string userCommand, NoobCodes & category);*/

/**/
/*
NoobScan::inspectArgs() NoobScan::inspectArgs()

NAME
        NoobScan::inspectArgs - organizes the inspection and action on user commands.
SYNOPSIS
        NoobCodes NoobScan::inspectArgs(string userCommand, NoobCodes & category);
            userCommand --> string holding user's desired command
            category    --> category of the command being asked
DESCRIPTION
        This function will parse the user command into individual items, and then inspect those items to ensure that the command is properly formatted. It does this by reviewing the primary command (main type of request from the user), and then seeing if the secondary commands match with it.
RETURNS
        NoobCodes value telling if the command was ultimately successfully formed (good) or failed based on layout (bad).
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::inspectArgs(string userCommand, NoobCodes & category){
    NoobCodes userRequest;
    
    // parse the argument into commands and ports
    userRequest = this->parseUserArgument(userCommand);
    
    // confirm the original parsing went through without error
    if(userRequest!=NoobCodes::success){
        reportError(userRequest);
        return userRequest;
    }
    
    // identify first argument
    userRequest = this->reviewPrimaryCommand();
    
    // regardless of outcome, categorize the overall request
    category = userRequest;
    
    // if first argument doesn't match, return a fail code
    if(userRequest==NoobCodes::fail){
        return userRequest;
    }
    
    // confirm secondary arguments are well formed, and act accordingly
    userRequest = this->reviewSecondaryCommands(userRequest);

    return userRequest;
}
/*NoobCodes inspectArgs(string userCommand, NoobCodes & category);*/

/**/
/*
NoobScan::parseUserArgument() NoobScan::parseUserArgument()

NAME
        NoobScan::parseUserArgument - parses user command string
SYNOPSIS
        NoobCodes NoobScan::parseUserArgument(string userCommand);
            userCommand --> command initially input by the user
DESCRIPTION
        This function will take the user's command string and break it into it's individual pieces. Originally, this was not used when creating this program , and we relied entirely on regex to parse the user command. After some tinkering, I decided that it would be more efficient to parse and then act on the string. Parsing will also allow me to set up future regexes that may provide for additional functionality.
        The function breaks the string into a vector of strings, which it then pushes into the appropriate class command vectors that the program will act on (vectors for URLs, command text, and port numbers).
        The unfortunate thing about this function is that, once I decided to parse the arguments, I had to part ways with my more advanced regexes, as simpler ones would do the trick without guesswork. I'm leaving the old ones in there because I think you'll appreciate taking a look at them (or maybe just because I was proud to have worked them out after a lot of trial and error). Had I known that I would not need those original regexes, I would have finished this project at least a week earlier, and probably with additional features. Really went down the rabbit hole on this one.
RETURNS
        NoobCode indicating the success of the overall command.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::parseUserArgument(string userCommand){
    
    // string holding feedback to the user, when feedback text is on
    string userFeedback;
    userFeedback.clear();
    
    // vector to hold user command, split by whitespace
    vector<string> userCommandParsed;
    
    // TODO: remove formatted URLs from the string
    
    // ensure any previous commands are empty
    clearCommandVectors();
    
    // set up regex to match commands with what we're looking for
    smatch matches;
    
    // this searches for words only (no numbers/white space)
    /* breakdown of Regex info, for those curious:
    \\b word boundary at the start (non-word before word)
     [^\\d\\W]+ no digits or whitespace
     \\b word boundary at the end (space after word)
    */
    //regex commandHunter("\\b[^\\d\\W]+\\b");
    regex commandHunter("[a-zA-Z]+");
    
    // this searches for numbers only (it's how we identify ports)
    //regex portHunter("\\b[0-9]{1,}");
    /* breakdown of Regex info, for those curious:
     [^\\.] Ensure number isn't preceeded by a '.' to avoid IP confusion
     \\b[0-9]+ look for numbers 0-9 (as many in a row as you'd like) at the start
     \\b(?!\\.) Ensure the number doesn't end with a '.' to avoid IP confusion
     */
    regex portHunterOLD("[^\\.]\\b[0-9]+\\b(?!\\.)");
    
    // alternative to portHunter if we're going the split route
    regex portHunter("(\\d+)");
    
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
    
    /* This searches for a URL
     (?:http(s)?:\/\/) Looks for optional http(s):// prefixes
     ?[\w.-]+ Looks for optional www.-esque entry
     (?:\.[\w\.-]+)+ words with dots or dashes after them (snanme.nmap.org)
     [\w\-\._~:/?#[\]@!\$&'\(\)\*\+,;=.]+ words with other symbols and backslashes
     */
    regex URLHunter(R"((?:http(s)?:\/\/)?[\w.-]+(?:\.[\w\.-]+)+[\w\-\._~:/?#[\]@!\$&'\(\)\*\+,;=.]+)");
                     
    //split user command to act on individually spaced words
    this->splitString(userCommand, userCommandParsed);
    
    bool firstError=true;
    for(auto const & commandEntry: userCommandParsed){
        
        // if a word command is found
        if(regex_match(commandEntry, commandHunter)){
            // add to the parsed user commands
            this->parsedCommand.push_back(commandEntry);
        }
        
        // if an IP address is recognized
        else if(regex_match(commandEntry, ipHunter)){
            // add the IP string to the commands
            this->parsedCommand.push_back(commandEntry);
            this->ipToScan.push_back(commandEntry);
        }
        
        // if a URL is recognized
        else if(regex_match(commandEntry, URLHunter)){
            // convert URL to IP
            string URLtoIP = ourScanner->getTargetIP(commandEntry);
            // if conversion was not successful, output to user and exit
            if(URLtoIP.empty()){
                userFeedback = "Your formatting is off (URL to IP conversion issue for " + commandEntry + "). Please try again\n";
                outputFeedback(userFeedback);
                return NoobCodes::failURL;
            }
            // if URL converted successfully, add to appropriate vectors
            else{
                userFeedback = "\t\tIP is " + URLtoIP + "\n";
                outputFeedback(userFeedback);
                this->parsedCommand.push_back(URLtoIP);
                this->siteToScan.push_back(commandEntry);
                this->ipToScan.push_back(URLtoIP);
            }
        }
        
        // if a number (port) is recornized)
        else if(regex_match(commandEntry, portHunter)){
            // attempt to convert string of port numbers to unsigned vector
            try {
                // if successful, push back to vector
                unsigned long ourPort = stoul(commandEntry);
                portsToScan.push_back((unsigned int)ourPort);
            } catch (const invalid_argument) {
                // if unsuccessful, confirm that any extra ports were moved off, and note the error. This is more of a failsafe than anything else, in case we've miscalculated for something.
                if(firstError){
                    portsToScan.pop_back();
                    firstError=false;
                }
                userFeedback = "Your formatting is off (misreading for " + commandEntry + "). Please try again\n";
                outputFeedback(userFeedback);
                return NoobCodes::failPort;
            }
        }
        else{
            // no need to do anything here. If the item isn't caught, it's likely a space, or bad user formatting.
        }
    }
    
    return NoobCodes::success;
    
}
/*NoobCodes parseUserArgument(string userCommand);*/

/**/
/*
NoobScan::reviewPrimaryCommand() NoobScan::reviewPrimaryCommand()

NAME
        NoobScan::reviewPrimaryCommand - reviews the initial command of the user
SYNOPSIS
        NoobCodes NoobScan::reviewPrimaryCommand()
DESCRIPTION
        This function reviews the initial command of the user, and returns the general command type that they're asking for. For instance, if "scan" is recognized, we know the user is asking for a scan command.
RETURNS
        NoobCodes enum indicating if the command failed, or what type of command is being asked.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
    else if(parsedCommand[0].compare("ipcheck")==0){
        return NoobCodes::IPRequest;
    }
    else{
        cout << "Command not recognized. Try again.\n";
        return NoobCodes::fail;
    }
}
/*NoobCodes reviewPrimaryCommand();*/

/**/
/*
NoobScan::reviewSecondaryCommands() NoobScan::reviewSecondaryCommands()

NAME
        NoobScan::reviewSecondaryCommands - reviews the secondary commands of the user
SYNOPSIS
        NoobCodes NoobScan::reviewSecondaryCommands(NoobCodes commandType);
            commandType --> indicates the type of command originally requested
DESCRIPTION
        This function will review the command type originally requested, and act on it. If it's a request to exit, the program will simply begin exiting. If it's a request for anything else, further functions will be called that will check to ensure that genre of command is executed properly.
RETURNS
        NoobCodes indicating whether or not the commend executed successfully.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::reviewSecondaryCommands(NoobCodes commandType){
    switch (commandType) {
        case NoobCodes::helpRequest:
            commandType = this->helpRequestCheck();
            break;
        case NoobCodes::scanRequest:
            commandType = this->scanRequestCheck();
            break;
        case NoobCodes::settingsRequest:
            commandType = this->settingsRequestCheck();
            break;
        case NoobCodes::debugRequest:
            this->debug();
            commandType=NoobCodes::success;
            break;
        case NoobCodes::IPRequest:
            commandType = this->IPRequestCheck();
            break;
        case NoobCodes::exitRequest:
            cout << "Exiting...\n";
            commandType=NoobCodes::success;
            break;
        default:
            cout << "Unknown command type.\n";
            return NoobCodes::fail;
            break;
    }
    return commandType;
}
/*NoobCodes reviewSecondaryCommands(NoobCodes commandType);*/

/**/
/*
NoobScan::reportError() NoobScan::reportError()

NAME
        NoobScan::reportError - reports errors to user screen
SYNOPSIS
        void NoobScan::reportError(NoobCodes programError);
            programError --> NoobCodes enum indicating the type of error
DESCRIPTION
        This function will receive a NoobCodes error, and translate it in english for the user to read, so they know what kind of error they've run into. I was on the cusp of learning how to do this through I/O overloading, but was not able to learn it in time. I will work to relocate this functionality post-graduation.
RETURNS
        void - no return
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// report errors to user screen
void NoobScan::reportError(NoobCodes programError){
    cout << "\n\t";
    switch(programError){
        case NoobCodes::fail:
            cout << "Miscellaneous failure reported\n";
            break;
        case NoobCodes::failURL:
            cout << "URL-related failure reported\n";
            break;
        case NoobCodes::failIP:
            cout << "IP-related failure reported\n";
            break;
        case NoobCodes::failPort:
            cout << "Port-related failure reported\n";
            break;
        case NoobCodes::socketError:
            cout << "Miscellaneous error with socket\n";
            break;
        case NoobCodes::socketCreationError:
            cout << "Socket creation error\n";
            break;
        case NoobCodes::socketCreationErrorICMP:
            cout << "ICMP socket creation error\n";
            break;
        case NoobCodes::socketCreationErrorDGRAM:
            cout << "DGRAM (UDP) socket creation error\n";
            break;
        case NoobCodes::socketCreationErrorSTREAM:
            cout << "TCP socket creation error\n";
            break;
        case NoobCodes::portConnectionError:
            cout << "Error establishing port connection\n";
            break;
        case NoobCodes::portNumberInvalid:
            cout << "Port number formatting issue\n";
            break;
        case NoobCodes::portSendError:
            cout << "Error sending to port\n";
            break;
        case NoobCodes::portReceiveError:
            cout << "Error receiving from port\n";
            break;
        case NoobCodes::IPBindingIssue:
            cout << "Error binding to IP\n";
            break;
        case NoobCodes::hostNameIssue:
            cout << "Error with host name\n";
            break;
        default:
            cout << "\n\tUnknown error. Sorry!\n";
    }
}
/*void reportError(NoobCodes programError);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::IPRequestCheck(){
    NoobCodes IPOutcome = NoobCodes::success;
    // set up temporary strings to hold the user URL, and the returning IP address
    string userURL="";
    string foundIP="";
    
    cout << "We're checking for an IP, now... \n";
    
    // If user potentially input a URL
    if(!siteToScan.empty()){
        
        // copy URL input (for later output)
        userURL = siteToScan[0];
        
        // retrieve IP address
        foundIP = this->ourScanner->getTargetIP(siteToScan[0]);
    }
    
    // If the user didn't put in a URL, prompt them
    else{
        
        // prompt user for URL
        cout << "\tPlease enter the URL: ";
        userURL = this->promptUser(false);
        
        // transfer IP address in plaintext
        foundIP = this->ourScanner->getTargetIP(userURL);
    }
    
    // if the IP returned was NULL
    if(foundIP.empty()){
        cout << "\tIP could not be retrieved for " << userURL << ". Please confirm formatting, and try again.\n";
        IPOutcome = NoobCodes::failPartial;
    }
    else{
        // output IP to user
        cout << "\tIP address result for " << userURL << ": " << foundIP << endl;
    }
    return IPOutcome;
}
/*NoobCodes IPRequestCheck();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::helpRequestCheck(){
    NoobCodes helpOutcome = NoobCodes::success;
    
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
                helpOutcome=NoobCodes::failPartial;
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
                helpOutcome=NoobCodes::failPartial;
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
            helpOutcome=NoobCodes::failPartial;
            break;
        default:
            break;
    }
    
    return helpOutcome;
}
/*NoobCodes helpRequestCheck();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::scanRequestCheck(){
    outputFeedback("Scan request registered\n");
    
    // holder for outcome of request
    NoobCodes outcome = NoobCodes::success;
    
    // holder for scan type
    NoobCodes scanType;
    
    // holds count for parsedCommand entries
    size_t parsedCount=0;
    
    parsedCount=parsedCommand.size();
    
    string scanTarget;
    
    // if 3 arguments (scan [scan type] [destination]), the command is most likely sufficiently formed. Remember: ports are not part of the parsedCommand.
    if(parsedCount == 3){
        // check scan type
        scanType=checkScanType();
        
        // run scan
        if(scanType==NoobCodes::tcp){
            this->ourTCPScan = new TCPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
            this->ourTCPScan->runMultiScan(portsToScan,ipToScan[0]);
            delete this->ourTCPScan;
        }
        else if(scanType==NoobCodes::udp){
            // instantiate UDP scanner
            this->ourUDPScan = new UDPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
            
            // run the scan
            this->ourUDPScan->runMultiScan(portsToScan, this->getIsRoot(), ipToScan[0]);
            //this->ourUDPScan->runScan(portsToScan[0], getIsRoot());
            
            // delete the instance of our scanner
            delete this->ourUDPScan;
        }
        else{
            cout << "Scan type currently unavailable.\n";
        }
        
    }
    
    // if 4 arguments (scan [scan type] [destination] [port group]), the command is most likely sufficiently formed. Remember: ports are not part of the parsedCommand. Port groups, however, are, since we interpret this as a string.
    else if(parsedCount == 4){
        // check scan type
        scanType=checkScanType();
        
        // adjust port group accordingly
        this->portsToScan = ourScanner->returnPortGroup(parsedCommand[3]);
        if(portsToScan.empty()){
            outcome = NoobCodes::fail;
            return outcome;
        }
        
        // run scan
        if(scanType==NoobCodes::tcp){
            this->ourTCPScan = new TCPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
            this->ourTCPScan->runMultiScan(portsToScan,ipToScan[0]);
            delete this->ourTCPScan;
        }
        else if(scanType==NoobCodes::udp){
            // instantiate UDP scanner
            this->ourUDPScan = new UDPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
            
            // run the scan
            this->ourUDPScan->runMultiScan(portsToScan, this->getIsRoot(), ipToScan[0]);
            
            // delete the instance of our scanner
            delete this->ourUDPScan;
        }
        else{
            cout << "Scan type currently unavailable.\n";
            outcome = NoobCodes::fail;
        }
    }
    
    // if less than 3 arguments, the command is malformed
    else{
        cout << "It looks like you might be having some trouble formulating your scan. Please follow the scan directions:" << endl;
        this->ourHelper->returnInfo("scan");
        outcome=NoobCodes::fail;
    }
    
    return outcome;
}
/*NoobCodes scanRequestCheck();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::settingsRequestCheck(){
    outputFeedback("Settings request registered\n");
    
    // NoobCodes return for settings response
    NoobCodes settingsOutcome = NoobCodes::success;
    
    // count parsedCommand entries
    size_t parsedCount=0;
    parsedCount=parsedCommand.size();
    
    // if user is requesting settings for a port
    if(portsToScan.size()!=0){
        // Inform them they have formatted their request incorrectly
        cout << "Port numbers do not apply to settings.\n\n";
        
        // note that they did not format their original argument perfectly
        settingsOutcome = NoobCodes::failPartial;
        
        // dump to general settings menu.
        displaySettings();
    }
    // if user is requesting a specific setting to toggle
    else if(parsedCount==2){
        // let the user know the setting they're requesting info for.
        string debugString = "You're asking specifically to access the setting for: " + parsedCommand[1] + "\n";
        outputFeedback(debugString);
        
        // check if user's command corresponds to a setting, and react appropriately
        settingsOutcome = this->displaySettings(findSettingsRequestType(parsedCommand[1]));
    }
    // if user just entered "settings"
    else if(parsedCount==1){
        // general settings request - show all settings menu
        outputFeedback("You're asking for general settings.\n\n");
        this->displaySettings(NoobCodes::settingsRequest);
    }
    // if there's some other type of issue with the user's settings request
    else{
        // output issue to user, and send them back
        cout << "Your settings request was formatted incorrectly. Either input \"settings\", or \"settings [specific setting topic]\"\n\n";
        settingsOutcome=NoobCodes::fail;
    }
    
    return settingsOutcome;
}
/*NoobCodes settingsRequestCheck();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
    else if(userRequest=="scan" || userRequest=="assist"){
        return NoobCodes::settingsForAssistedScan;
    }
    else{
        return NoobCodes::fail;
    }
}
/*NoobCodes findSettingsRequestType(string userRequest);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes NoobScan::displaySettings(NoobCodes settings){
    NoobCodes displaySettingsResult = NoobCodes::success;
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
            
            // allow user to adjust scan delays
            settingsDelay(userAnswer, settings);
            
            // jump to the top of the menu
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
            cout << "Let's walk you through a scan ... \n";
            
            // send to ScanAddress slow scan function
            this->settingsAssisted(userAnswer, settings);
            
            // exit this settings menu
            continue;
        }
        else if(settings == NoobCodes::exitRequest){
            cout << "Exiting settings ... \n";
            settings = NoobCodes::exitRequest;
            break;
        }
        
        // otherwise, output the correct options for the user to use
        else{
            // output other settings options for user to retry
            cout << "Invalid settings choice. Please select from the following:\n"
            << setw(11) << left << "\tassist" << "\t-\tcreate a step by step scan for 1 port\n"
            << setw(11) << left << "\tdelay" << "\t-\tset delay between ports to scan\n"
            << setw(11) << left << "\tdictionary" << "\t-\tadd terms to dictionary\n"
            << setw(11) << left << "\tfeedback" << "\t-\toutput additional text to console during command response\n"
            << setw(11) << left << "\tgroups" << "\t-\tview and change scan groups\n"
            << setw(11) << left << "\trecorder" << "\t-\tenable or disable the recorder\n"
            << setw(11) << left << "\tscan" << "\t-\tcreate an assisted scan\n"
            << setw(11) << left << "\ttimeout" << "\t-\tset timeout length when waiting for server to respond\n" << endl;
            displaySettingsResult = NoobCodes::fail;
            break;
        }
    }
    //return NoobCodes::restart;
    return displaySettingsResult;
}
/*NoobCodes displaySettings(NoobCodes settings=NoobCodes::settingsRequest);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void settingsOptions(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::settingsAssisted(int &userAnswer, NoobCodes &settings){
    int portToScan=1;
    
    string targetURL;
    
    string targetIP;
    
    NoobCodes scanType;
    
    
    // ask user what kind of scan they want to do
    cout << "What kind of scan would you like to conduct?\n";
    cout << "\t1. TCP\n";
    cout << "\t2. UDP\n";
    
    userAnswer = getValidInput(1,2);
    
    // store scan result
    if(userAnswer==1){
        scanType=NoobCodes::tcp;
    }
    else{
        scanType=NoobCodes::udp;
    }
    
    // ask user if they want to enter a URL or IP address
    
    cout << "How would you like to identify your target?\n";
    cout << "\t1. URL\n";
    cout << "\t2. IP\n";
    
    userAnswer = getValidInput(1,2);
    
    // if URL, convert to IP and store
    if(userAnswer==1){
        do{
            // clear the target URL for another try
            targetURL.clear();
            
            // prompt user for a valid URL and convert it to an IP accordingly
            cout << "Please enter a valid url, or type \"quit\" to quit:\n";
            
            // prompt user for the URL
            targetURL=promptUser();
            
            // if there was an issue with the prompt (cin.ignore, getline issues, etc), ask again.
            if(targetURL.empty()){
                flush(cout);
                targetURL = promptUser(false);
            }
            
            cout << "\tYou entered: " << targetURL << endl;
            
            // convert it
            targetIP=ourScanner->getTargetIP(targetURL);
            
            // if user aborts the process
            if(targetURL.compare("quit")==0){
                cout << "\tQuitting ... \n";
                settings=NoobCodes::restart;
                return;
            }
            // confirm a proper IP came back
            if(targetIP.empty()){
                cout << "\tURL does not lead to an IP address. Please check formatting." << endl;
            }
            flush(cout);
        }while(targetIP.empty());
        
        
        cout << "\tIP for " << targetURL << " is " << targetIP << endl;
    }
    
    // if IP, store IP (we'll trust the user here)
    else{
        cout << "Please enter the IP:\n";
        
        // prompt user for IP
        targetIP=promptUser();
        
        // if there was an issue with the prompt (cin.ignore, getline issues, etc), ask again.
        if(targetIP.empty()){
            flush(cout);
            targetIP = promptUser(false);
        }
    }
    
    // have user enter the port to scan
    cout << "Enter 1 port number to scan:\n";
    portToScan = getValidInput(1,65535);
    
    // run the scan
    if(scanType==NoobCodes::tcp){
        this->ourTCPScan = new TCPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
        this->ourTCPScan->runScan(portToScan, targetIP);
        delete this->ourTCPScan;
    }
    else{
        this->ourUDPScan = new UDPScanner(ourScanner->getSleepTimer(), ourScanner->getTimeoutTimer(), ourScanner->getVariableDelayStatus());
        this->ourUDPScan->runScan(portToScan, this->getIsAdmin());
        delete this->ourUDPScan;
    }
    
    settings = NoobCodes::restart;
    
    return;
}
/*void settingsAssisted(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void settingsDictionary(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::settingsDelay(int &userAnswer, NoobCodes &settings){
    // display delay time options (variable, or enter specific number)
    cout << "Your current delay time between ports is "
        << this->ourScanner->getSleepTimer() << endl << endl;
    cout << "\t1. Set specific delay time\n"
        << "\t2. Enable variable delay time between scans.\n"
        << "\t3. Exit (keep current delay setting)\n";
    userAnswer = getValidInput(1,3);
    if(userAnswer==1){
        int delayTime;
        cout << "Please enter new delay time between port scans (microseconds between 0 and 100000): ";
        delayTime = getValidInput(0,100000);
        this->ourScanner->setSleepTimer(delayTime);
        cout << "Your new delay time between ports is "
            << this->ourScanner->getSleepTimer() << endl;
        settings = NoobCodes::settingsForDelay;
    }
    else if(userAnswer==2){
        int variableAnswer;
        // notify user of scan status
        cout << "Variable scan is currently ";
        if(ourScanner->getVariableDelayStatus()){
            cout << "on.\n";
        }
        else{
            cout << "off.\n";
        }
        
        // prompt user to enable or disable the status
        cout << "\t1. Turn variable scan times on.\n"
            << "\t2. Turn variable scan times off.\n"
            << "\t3. Exit (keep current setting)\n";
        variableAnswer = getValidInput(1,3);
        if(variableAnswer==1){
            ourScanner->setVariableDelayStatus(true);
        }
        if(variableAnswer==2){
            ourScanner->setVariableDelayStatus(false);
        }
        cout << "Variable scan is currently ";
        if(ourScanner->getVariableDelayStatus()){
            cout << "on.\n";
        }
        else{
            cout << "off.\n";
        }
        settings = NoobCodes::settingsForDelay;
    }
    else{
        settings = NoobCodes::restart;
    }
    
    return;
    
}
/*void settingsDelay(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
        << "\t3. Exit (keep current recorder setting).\n"
        << "\t4. Output record to screen.\n"
        << "\t5. Output record to file.\n";
    userAnswer = getValidInput(1,5);
    
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
    else if(userAnswer==4){
        this->userRecorder->showHistory();
        settings = NoobCodes::restart;
    }
    else if(userAnswer==5){
        this->userRecorder->moveHistoryToFile();
        cout << "\tHistory attempt sent to destination (desktop default).\n";
        settings = NoobCodes::restart;
    }
    else{
        cout << "\tExiting recorder settings.\n";
        settings = NoobCodes::restart;
    }
    
    return;
}
/*void settingsRecorder(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// displays the timeouts options
void NoobScan::settingsTimeouts(int & userAnswer, NoobCodes & settings){
    cout << "The current timeout limit for server response is: " << this->ourScanner->getTimeoutTimer() << " seconds.\n";
    cout << "Please enter your new time (between 1 and 10 seconds): " << endl;
    
    unsigned int newTime = getValidInput(1,10);
    
    this->ourScanner->setTimeoutTimer(newTime);
    
    cout << "The new timeout limit for server response is: " << this->ourScanner->getTimeoutTimer() << " seconds.\n";
    
    settings = NoobCodes::restart;
    
    return;
}
/*void settingsTimeouts(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void settingsDebug(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
            if(!portsToScan.empty()){
                ourScanner->setCustomList(this->portsToScan);
            }
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
/*void settingsGroups(int & userAnswer, NoobCodes & settings);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// pull all numbers from a string, and place them into a vector
void NoobScan::getNums(vector<unsigned> & ourNums){
    // set up variables to capture numbers from string
    vector<string> splitString;
    unsigned portHolder;
    string ourString;
    smatch matches;
    
    // a simple regex of all digits
    regex numberHunter("(\\d+)");
    bool tryAgain = false;
    
    do{
        // if this isn't our first time in the loop
        if(tryAgain){
            // re-prompt user
            cout << "Please try entering your port numbers again: ";
        }
        
        // remove old values from strings/vectors
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
        
        // use a ternary operator to show that I can (even though they're less readable)!
        tryAgain = (this->getValidInput(1,2)==1)? false : true;
        
    }while(tryAgain);
    
    ourScanner->setCustomList(ourNums);
    
    return;
}
/*void getNums(vector<unsigned> & ourNums);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void splitString(string originString, vector<string> & splitString);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
        outputFeedback("Something went wrong ... \n");
        return NoobCodes::fail;
    }
}
/*NoobCodes checkScanType();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// turn on and off feedback
void NoobScan::setSystemFeedback(bool isOn){
    this->systemFeedback=isOn;
}
/*void setSystemFeedback(bool isOn);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// turn on and off root indicator
void NoobScan::setRoot(bool rootStatus){
    this->isRoot=rootStatus;
}
/*void setRoot(bool rootStatus);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// turn on and off admin indicator
void NoobScan::setAdmin(bool adminStatus){
    this->isAdmin=adminStatus;
}
/*void setAdmin(bool adminStatus);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// turn on and off variable scan indicator
void NoobScan::setVariableScan(bool isOn){
    this->variableTime=isOn;
}
/*void setVariableScan(bool isOn);*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// check if variable scan is on
bool NoobScan::getVariableScanStatus(){
    return this->variableTime;
}
/*bool getVariableScanStatus();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// check if feedback is on
bool NoobScan::getSystemFeedback(){
    return this->systemFeedback;
}
/*bool getSystemFeedback();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool NoobScan::getIsRoot(){
    return this->isRoot;
}
/*bool getIsRoot();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool NoobScan::getIsAdmin(){
    return this->isAdmin;
}
/*bool getIsAdmin();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// clears the user commands between requests
void NoobScan::clearCommandVectors(){
    this->parsedCommand.clear();
    this->portsToScan.clear();
    this->ipToScan.clear();
    this->siteToScan.clear();
    return;
}
/*void clearCommandVectors();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void NoobScan::displayUserCommands(){
    cout << "\tCommands input:\t";
    for(const auto & i: this->parsedCommand){
        cout << i << " ";
    }
    cout << endl;
    return;
}
/*void displayUserCommands();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
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
/*void displayUserPortRequests();*/

/**/
/*


NAME

SYNOPSIS
 
DESCRIPTION
 
RETURNS

AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// code I'm experimenting with, or have thrown away
void NoobScan::debug(int debugPort){

    userRecorder->showHistory();
    userRecorder->moveHistoryToFile();

}
