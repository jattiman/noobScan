/**/
/*
 ScanAddress.cpp of noobScan, a novice port scanner
 Author: John Atti
 Completed 8/16/2020
*/
/**/

#include "ScanAddress.h"

/**/
/*
ScanAddress::ScanAddress() ScanAddress::ScanAddress()
 
NAME
        ScanAddress::ScanAddress - class default constructor
SYNOPSIS
        ScanAddress::ScanAddress()
DESCRIPTION
        Constructs the ScanAddress class object, initializing variables to default settings
RETURNS
        Class object.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
ScanAddress::ScanAddress(){
    this->sleepTimer=0;
    this->timeOut=1;
    this->retries=2;
    this->ourInterface="en0";
}
/*ScanAddress();*/

/**/
/*
ScanAddress::getValidInput() ScanAddress::getValidInput()

NAME
        ScanAddress::getValidInput - confirm user input is valid
SYNOPSIS
        int ScanAddress::getValidInput(int minNum, int maxNum)
            int minNum --> minimum acceptable number
            int maxNum --> maximum acceptable number
DESCRIPTION
        Function prompts the user for a integer input, handles improperly formatted input, and ensure the integer accepted is sufficient for the prompt given to the user elsewhere in the program. User will be continually prompted until a valid input is entered.
RETURNS
        Integer with confirmed, appropriate user choice.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
int ScanAddress::getValidInput(int minNum, int maxNum){
    int userInput;
    cout << ">: ";
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
ScanAddress::checkScanType() ScanAddress::checkScanType()

NAME
        ScanAddress::checkScanType - confirm the scan type being requested
SYNOPSIS
        char ScanAddress::checkScanType(string scanCommand);
            scanCommand --> the string of the command given to the user
DESCRIPTION
        This function is in place to eventually intake a string command from the user, and convert it to a char value that would be used to flater functions. Note: this is not yet implemented, but will once I advance this project further, post-graduation.
RETURNS
        Returns char representing the scan to be performed.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
char ScanAddress::checkScanType(std::string scanCommand){
    //TODO: translate user commands for more advance scanning detection
    return 'a';
}
/*char checkScanType(string scanCommand);*/

/**/
/*
ScanAddress::initiateScan() ScanAddress::initiateScan()

NAME
        ScanAddress::initiateScan - initiate scan, report results and output to screen
SYNOPSIS
        string ScanAddress::initiateScan(char ourScanType);
            ourScanType --> character representing the scan type to be performed.
DESCRIPTION
        This function is in place to eventually intake a character command representing the desired scan to be performed, and will eventually perform the scan based on it. Note: this is not yet implemented, but will once I advance this project further, post-graduation.
RETURNS
        String representing the scan outcome.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
string ScanAddress::initiateScan(char ourScanType){
    switch (ourScanType) {
        case 't':
            
            break;
            
        default:
            break;
    }
    return "string";
}
/*string initiateScan(char ourScanType);*/

/**/
/*
ScanAddress::getTargetIP() ScanAddress::getTargetIP()

NAME
        ScanAddress::getTargetIP - resolve host name to IP address
SYNOPSIS
        string ScanAddress::getTargetIP(string targetHost);
            targetHost --> a string of the host URL to be converted to an IP address
DESCRIPTION
        This function intakes a URL string and attempts to get the IP address associated with it. If the string is malformed or the IP address cannot be found (network issue), the function will return a null string ({}). If the URL is able to be converted, an IPv4 address will be generated for it.
RETURNS
        String value holding the IP address of the URL, or blank if no IP address can be found.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
string ScanAddress::getTargetIP(string targetHost){
    
    //TODO: error check URL here, or trust user
        
    // create host structure to convert URL to IP address
    struct hostent *ourHost;
    string targetIPAddress;
    
    // convert (store) the URL accordingly
    ourHost=gethostbyname(targetHost.c_str());
        
    // if issue with conversion, return blank string
    if(ourHost == NULL){
        //cout << "Null address.\n";
        return {};
    }
    // otherwise, return the IP address
    else{
        targetIPAddress = inet_ntoa(*((struct in_addr*) ourHost->h_addr_list[0]));
        return targetIPAddress;
    }
}
/*string getTargetIP(string targetHost);*/

/**/
/*
ScanAddress::getHostIP() ScanAddress::getHostIP()

NAME
        ScanAddress::getHostIP - retrieve user IP address
SYNOPSIS
        string ScanAddress::getHostIP(string ifaNamePreference);
            ifaNamePreference --> string value holding the interface that the IP will be searched under ('en0' in this case)
DESCRIPTION
        This function will iterate through the machine's network interfaces, match the ifaddrs name type for the IP we're looking for, retrieve it, and return the IP value of the host machine.
        If there is any issue, a "blank" return value ({}) will be sent. This value is needed, as opposed to NULL, based on issues encountered when NULL is returned.
RETURNS
        String value representing host machine IP. Blank if no IP could be found (network issues).
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
string ScanAddress::getHostIP(string ifaNamePreference){
    // create address structure for IP
    struct ifaddrs *ifaPlaceholder;
    struct ifaddrs *ifaInspector;
    struct sockaddr_in *sa;
    string addr;
    string ourHostIP;

    // attempt to get list of machine's network interfaces

    // if this fails, abandon and report
    if(getifaddrs (&ifaPlaceholder)==-1){
        cout << "\tError finding network interfaces.\n";
        return {};
    }
    // if it succeeds, find the IP for the preferred interface
    else{
        // look through all interfaces on machine
        for (ifaInspector = ifaPlaceholder; ifaInspector; ifaInspector = ifaInspector->ifa_next) {
            // ensure that the interface matches what we're looking for, and pull IP address from it, if so
            if (ifaInspector->ifa_addr && ifaInspector->ifa_addr->sa_family==AF_INET) {
                sa = (struct sockaddr_in *) ifaInspector->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                if(ifaInspector->ifa_name == ifaNamePreference){
                    ourHostIP=addr;
                }

            }
        }
        freeifaddrs(ifaPlaceholder);
        //TODO: if ourHostIP is empty, prompt user to enter different interface, or return {} as above with error
        return ourHostIP;
    }
}
/*string getHostIP(string ifaNamePreference = "en0");*/

/**/
/*
ScanAddress::addPortList() ScanAddress::addPortList()

NAME
        ScanAddress::addPortList - add a port to the port list
SYNOPSIS
        void ScanAddress::addPortList(int newPortNumber, vector<int> portVector);
            newPortNumber --> int of the port number to be added
            portVector --> vector that the port will be added to
DESCRIPTION
        This function will add a port number to a vector of choice. For instance, if there is a newly discovered open port, this function can add that port to the openPort vector.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::addPortList(int newPortNumber, vector<int> portVector){
    portVector.emplace_back(newPortNumber);
    return;
}
/*void addPortList(int newPortNumber, vector<int> portVector);*/

/**/
/*
ScanAddress::getPortList() ScanAddress::getPortList()

NAME
        ScanAddress::getPortList - list ports being scanned
SYNOPSIS
        void ScanAddress::getPortList();
DESCRIPTION
        This function will display the ports that the user is about to scan. If no ports have been placed in the vector, the user will be informed that nothing is selected. If the size of the ports selected is small, one port per line will be printed. If there are a large number of ports being scanned, they'll be printed next to each other on the same line.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::getPortList(){
    
    // if the vector is empty, don't try to use it
    if(this->portsToScan.size()==0){
        cout << "You have not selected default ports to scan.\n";
    }
    
    // if a small number of ports are selected, print them vertically
    else if(portsToScan.size()<20){
        for(auto const & portNum : this->portsToScan){
            cout << portNum << endl;
        }
    }
    
    // if a large number of ports are selected, print them on a line
    else{
        for(auto const & portNum : this->portsToScan){
            cout << portNum << "\t";
        }
    }
    return;
}
/*void getPortList();*/

/**/
/*
ScanAddress::portValidityCheck() ScanAddress::portValidityCheck()

NAME
        ScanAddress::portValidityCheck - check port number validity
SYNOPSIS
        NoobCodes ScanAddress::portValidityCheck(int portNumToCheck);
            portNumToCheck --> int holding the port number to review
DESCRIPTION
        Function intakes a port number, and ensures that it is within the acceptable port number range.
RETURNS
        NoobCodes enum representing if the port number appropriate or if it cannot be accepted.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
NoobCodes ScanAddress::portValidityCheck(int portNumToCheck){
    if(portNumToCheck < 0 || portNumToCheck > 65535){
        return NoobCodes::portNumberInvalid;
    }
    return NoobCodes::success;
}
/*NoobCodes portValidityCheck(int portNumToCheck);*/

/**/
/*
ScanAddress::getSleepTimer() ScanAddress::getSleepTimer()

NAME
        ScanAddress::getSleepTimer - returns the sleep timer
SYNOPSIS
        unsigned int ScanAddress::getSleepTimer();
DESCRIPTION
        This returns the sleep timer stored in the class
RETURNS
        Unsigned int representing the sleep timer
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
unsigned int ScanAddress::getSleepTimer(){
    return sleepTimer;
}
/*unsigned int getSleepTimer();*/

/**/
/*
ScanAddress::getTimeoutTimer() ScanAddress::getTimeoutTimer()

NAME
        ScanAddress::getTimeoutTimer - get timeout wait time
SYNOPSIS
        unsigned int ScanAddress::getTimeoutTimer();
DESCRIPTION
        Function will return the timeout timer variable of the class, which represents the amount of time that scanners will wait for a port to reply to them.
RETURNS
        unsigned int representing the timeout delay time
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
unsigned int ScanAddress::getTimeoutTimer(){
    return this->timeOut;
}
/*unsigned int getTimeoutTimer();*/

/**/
/*
ScanAddress::getHostMac() ScanAddress::getHostMac()

NAME
        ScanAddress::getHostMac - returns host MAC address
SYNOPSIS
        string ScanAddress::getHostMac(string ifaNamePreference);
            ifaNamePreference --> string holding the interface name for its corresponding MAC address
DESCRIPTION
        This function will retrieve and convert a MAC address into human readable format from the host's machine. If no MAC address can be found, a blank ({}) string will be returned and the user will have the error output to the screen.
        This is macOS dependant, as macOS cannot use the same functions used on other UNIX variants (linux) to retrieve the MAC address.
        Although it might not be professional to say so in documentation, I must admit that the lack of macOS networking resources online was quite a pain to navigate in the beginning of this project.
RETURNS
        String variable representing the machine's MAC address.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
string ScanAddress::getHostMac(string ifaNamePreference){
    
    // define structures
    struct ifaddrs *ifaPlaceholder;
    struct ifaddrs *ifaInspector;
    
    // holder for the raw mac address
    unsigned char rawMac[6];
    
    // holder for the human readable mac address
    char ourMac[32]={0};
    
    // string to be returned
    string finalMac;
    
    // populate structures to hold mac address, transfer accordingly. Note: ioctl will not work on macOS as it would on LINUX, due to SIOCGIFHWADDR not being recognized on macOS currently. This is why we use this longer for loop to match the MAC with the interface being used, below.
    if (getifaddrs(&ifaPlaceholder) == 0) {
        
        // loop through interfaces
        for (ifaInspector = ifaPlaceholder; ifaInspector != NULL; ifaInspector = ifaInspector->ifa_next){
            
            // if we find the interface we're looking for
            if (ifaInspector->ifa_name == ifaNamePreference && ifaInspector->ifa_addr != NULL && ifaInspector->ifa_addr->sa_family == AF_LINK){
                
                // create structure to hold MAC address
                struct sockaddr_dl* sdl = (struct sockaddr_dl *)ifaInspector->ifa_addr;
                
                // grab the mac address into an unsigned char array
                if (6 == sdl->sdl_alen) {
                    memcpy(rawMac, LLADDR(sdl), sdl->sdl_alen);
                    
                    // format MAC address appropriately. Note: %02x forces at least 2 digits to appear for each entry in the MAC address. This is for both formatting and the leading 0.
                    // use sprintf (instead of sstream) to copy address
                    sprintf(ourMac, "%02x:%02x:%02x:%02x:%02x:%02x",rawMac[0], rawMac[1], rawMac[2], rawMac[3], rawMac[4], rawMac[5]);
                    
                    // to make things easier on return, copy readable mac to string
                    finalMac = ourMac;
                }
            }
        }
        freeifaddrs(ifaPlaceholder);
        return finalMac;
    }
    
    // if getifaddrs fails, we return an empty string
    else {
        printf("Error accessing host machine network interface: %s\n", strerror(errno));
        return {};
    }
}
/*string getHostMac(string ifaNamePreference = "en0");*/

/**/
/*
ScanAddress::getRetries() ScanAddress::getRetries()

NAME
        ScanAddress::getRetries - get the number of times the port will be retried
SYNOPSIS
        int ScanAddress::getRetries();
DESCRIPTION
        Function returns the number of retries that will be allowed between attempts to determine socket open/closed. Mainly applies to UDP scanning, right now.
RETURNS
        Integer representing the retry count.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
int ScanAddress::getRetries(){
    return this->retries;
}
/*int getRetries();*/

/**/
/*
ScanAddress::getInterface() ScanAddress::getInterface()

NAME
        ScanAddress::getInterface - returns machine networking interface string
SYNOPSIS
        string ScanAddress::getInterface();
DESCRIPTION
        Function will return the string representing the network interface being searched for. Default to 'en0' for macOS wireless interfaces.
RETURNS
        String representing host network interface type.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
string ScanAddress::getInterface(){
    return this->ourInterface;
}
/*string getInterface();*/

/**/
/*
ScanAddress::getVariableDelayStatus() ScanAddress::getVariableDelayStatus()

NAME
        ScanAddress::getVariableDelayStatus - returns variable delay status
SYNOPSIS
        bool ScanAddress::getVariableDelayStatus();
DESCRIPTION
        This will return whether or not the user is having a delay in the scans sent out.
RETURNS
        Bool value representing if the variable delay option is turned on.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
bool ScanAddress::getVariableDelayStatus(){
    return this->variableDelay;
}
/*bool getVariableDelayStatus();*/

/**/
/*
ScanAddress::setCustomList() ScanAddress::setCustomList()

NAME
        ScanAddress::setCustomList - updates the custom list of ports that the user makes, to later scan.
SYNOPSIS
        void ScanAddress::setCustomList(vector<unsigned int> newList);
            newList --> vector of unsigned ints representing the new list of ports
DESCRIPTION
        This function will clear and then overwrite the custom vector of ports with a new vector, defined by the user. The user can redefine this list as many times as they'd like.
RETURNS
        Void - no return value.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::setCustomList(vector<unsigned int> newList){
    this->customList.clear();
    this->customList = newList;
}
/*void setCustomList(vector<unsigned> newList);*/

/**/
/*
ScanAddress::setSleepTimer() ScanAddress::setSleepTimer()

NAME
        ScanAddress::setSleepTimer - sets the sleep timer
SYNOPSIS
        void ScanAddress::setSleepTimer(unsigned int newSleepTime);
            newSleepTime --> unsigned int from user representing the new sleep time
DESCRIPTION
        Function allows the user to set a new sleep time, which will be translated into microseconds to our class' sleep timer.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::setSleepTimer(unsigned int newSleepTime){
    this->sleepTimer=newSleepTime;
    return;
}
/*void setSleepTimer(unsigned int newSleepTime);*/

/**/
/*
ScanAddress::setTimeoutTimer() ScanAddress::setTimeoutTimer()

NAME
        ScanAddress::setTimeoutTimer - sets the timeout limit for scanners
SYNOPSIS
        void ScanAddress::setTimeoutTimer(unsigned int newTimeout);
            newTimeout --> the amount of time to wait for timeout
DESCRIPTION
        Function allows the user to set a new timeout time, which will be used in future scans to wait for responses.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::setTimeoutTimer(unsigned int newTimeout){
    this->timeOut = newTimeout;
}
/*void setTimeoutTimer(unsigned int newTimeout);*/

/**/
/*
ScanAddress::setRetries() ScanAddress::setRetries()

NAME
        ScanAddress::setRetries - set the number of times the port will be retried
SYNOPSIS
        void ScanAddress::setRetries(int newRetryAmount);
            newRetryAmount --> number of times retries will be made before quitting
DESCRIPTION
        Function allows user to set a new number for the amount of retries allowed before a port is abandoned.
RETURNS
        Void - no return value.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::setRetries(int newRetryAmount){
    this->retries = newRetryAmount;
    return;
}
/*void setRetries(int newRetryAmount);*/

/**/
/*
ScanAddress::setInterface() ScanAddress::setInterface()

NAME
        ScanAddress::setInterface - set the user machine interface
SYNOPSIS
        void ScanAddress::setInterface(string ifType)
            ifType --> string representing new interface name
DESCRIPTION
        Function will clear current user interface value ('en0' by default) and place the new value in this class variable.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// set the user interface
void ScanAddress::setInterface(string ifType){
    
    // ensure any items in string are fully cleared to avoid oddities from arising
    this->ourInterface.clear();
    
    // assign new interface string
    this->ourInterface = ifType;
}
/*void setInterface(string ifType);*/

/**/
/*
ScanAddress::setVariableDelayStatus() ScanAddress::setVariableDelayStatus()

NAME
        ScanAddress::setVariableDelayStatus - turns on and off variable delay
SYNOPSIS
        void ScanAddress::setVariableDelayStatus(bool variableDelayStatus);
            variableDelayStatus --> bool representing if variable delay is on or off
DESCRIPTION
        Setter function to turn variable delay class variable on or off. Impacts whether or not there is an additional pause/wait time during scans.
RETURNS
        Void - no return.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::setVariableDelayStatus(bool variableDelayStatus){
    this->variableDelay = variableDelayStatus;
    return;
}
/*void setVariableDelayStatus(bool variableDelayStatus);*/

/**/
/*
ScanAddress::returnPortGroup() ScanAddress::returnPortGroup()

NAME
        ScanAddress::returnPortGroup - returns vector of port group to be scanned
SYNOPSIS
        vector<unsigned> ScanAddress::returnPortGroup(string portGroup);
            portGroup --> string variable for port group the user wants to scan
DESCRIPTION
        This function pulls a word form the user's initial command, determined to be a port group, and checks to see if a corresponding port group is available. If a group is found, the port groups generated by the system (or the user) will be returned. If no group is found, a blank scanning group will be returned and the error will be output to the screen.
RETURNS
        Returns a vector of unsigned numbers representing ports.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
vector<unsigned> ScanAddress::returnPortGroup(string portGroup){
    if(portGroup=="chat"){
        return this->chatPorts;
    }
    else if(portGroup=="gaming"){
        return this->gamingPorts;
    }
    else if(portGroup=="malicious"){
        return this->maliciousPorts;
    }
    else if(portGroup=="peer"){
        return this->peerPorts;
    }
    else if(portGroup=="popular"){
        return this->popularPorts;
    }
    else if(portGroup=="streaming"){
        return this->streamingPorts;
    }
    else if(portGroup=="custom"){
        return this->customList;
    }
    else{
        cout << "Your entered group name, " << portGroup << ", was not found. Try your command again.\n";
    }
    return this->portsToScan;
}
/*vector<unsigned> returnPortGroup(string portGroup);*/

/**/
/*
ScanAddress::generateNewSeed() ScanAddress::generateNewSeed()

NAME
        ScanAddress::generateNewSeed - Generates random number for variable scan
SYNOPSIS
        unsigned ScanAddress::generateNewSeed();
DESCRIPTION
        This function uses system time information to attempt to generate a more random number between 0 and 1000000 (will be used for microseconds later). I used this function in OPL to handle a portion of the coin toss randomness, and found that it worked well enough to reuse here!
RETURNS
        Unsigned number which is hopefully random enough to help our variable scan.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
// set seed with random number
unsigned ScanAddress::generateNewSeed(){
    // pull the time since epoch, and then mod it by 10000 (variable time up to 1 second)
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    return (seed%1000000);
}
/*unsigned generateNewSeed();*/

/**/
/*
ScanAddress::translateNoob() ScanAddress::translateNoob()

NAME
        ScanAddress::translateNoob - translate NoobCode to char
SYNOPSIS
        char ScanAddress::translateNoob(NoobCodes ourCode);
            ourCode --> NoobCode enum to be translated
DESCRIPTION
        This intakes our enum class and translates it into a character which will be used for our recorder, based on type (passing/failing results, request type, etc). I realize that I need to make this more efficient (there are a lot of codes), but based on this pandemic, I feel like I've been stuck in that 80/20 zone for a while, and when making this function, I honestly just wanted it to work! I hope you understand.
RETURNS
        Char value representing enum type.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
char ScanAddress::translateNoob(NoobCodes ourCode){
    char noobCategory = 'x';
    
    // translate noobcodes into pass/fail/scan type/etc
    switch (ourCode) {
        case NoobCodes::fail:
        case NoobCodes::failPartial:
        case NoobCodes::failURL:
        case NoobCodes::failIP:
        case NoobCodes::failPort:
        case NoobCodes::socketError:
        case NoobCodes::socketCreationError:
        case NoobCodes::socketCreationErrorICMP:
        case NoobCodes::socketCreationErrorDGRAM:
        case NoobCodes::socketCreationErrorSTREAM:
        case NoobCodes::portConnectionError:
        case NoobCodes::portConnectionDenied:
        case NoobCodes::portNumberInvalid:
        case NoobCodes::portSendError:
        case NoobCodes::portReceiveError:
        case NoobCodes::IPBindingIssue:
        case NoobCodes::hostNameIssue:
        case NoobCodes::portSendDenied:
            noobCategory = 'f';
            break;
        case NoobCodes::success:
        case NoobCodes::portConnectionSuccess:
            noobCategory = 'g';
            break;
        case NoobCodes::helpRequest:
            noobCategory = 'h';
            break;
        case NoobCodes::scanRequest:
            noobCategory = 's';
            break;
        case NoobCodes::IPRequest:
            noobCategory = 'i';
            break;
        case NoobCodes::debugRequest:
            noobCategory = 'd';
            break;
        case NoobCodes::settingsRequest:
            noobCategory = 'v';
            break;
        case NoobCodes::exitRequest:
        case NoobCodes::tcp:
        case NoobCodes::udp:
        case NoobCodes::fin:
        case NoobCodes::syn:
        case NoobCodes::settingsForDelay:
        case NoobCodes::settingsForDictionary:
        case NoobCodes::settingsForAssistedScan:
        case NoobCodes::settingsForRecorder:
        case NoobCodes::settingsForScanGroups:
        case NoobCodes::settingsForTimeouts:
        case NoobCodes::settingsForDebugText:
        case NoobCodes::restart:
            noobCategory = 'p';
            break;
        default:
            //cout << "Unknown case - that's odd.\n";
            noobCategory = 'z';
            break;
    }
    return noobCategory;
}
/*char translateNoob(NoobCodes ourCode);*/

/**/
/*
ScanAddress::debug() ScanAddress::debug()

NAME
        ScanAddress::debug - debug function
SYNOPSIS
        void ScanAddress::debug();
DESCRIPTION
        Holder for functions that I will be debugging in the future. I will not be removing this, even though it is blank, because I intend to continue utilizing it post-graduation.
RETURNS
        Void - none.
AUTHOR
        John Atti
DATE
        6:00 PM 8/16/2020
*/
/**/
void ScanAddress::debug(){
    return;
}
/*void debug();*/


