/**/
/*
 ScanAddress.cpp of noobScan, a novice port scanner
 Author: John Atti
 Completed 8/16/2020
*/
/**/

#include "ScanAddress.h"

ScanAddress::ScanAddress(){
    this->sleepTimer=0;
    this->timeOut=1;
    this->retries=2;
    this->ourInterface="en0";
}
/*ScanAddress();*/

// confirm user input is valid
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

// confirm the scan type being requested
char ScanAddress::checkScanType(std::string scanCommand){
    return 'a';
}
/*char checkScanType(string scanCommand);*/

// initiate scan, report results and output to screen
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

// resolve host name to IP address
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

// retrieve our IP address
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
            // ensure that the interface is
            if (ifaInspector->ifa_addr && ifaInspector->ifa_addr->sa_family==AF_INET) {
                sa = (struct sockaddr_in *) ifaInspector->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                if(ifaInspector->ifa_name == ifaNamePreference){
                    ourHostIP=addr;
                }

            }
        }
        freeifaddrs(ifaPlaceholder);
        return ourHostIP;
    }
}
/*string getHostIP(string ifaNamePreference = "en0");*/

// add a port to the port list
void ScanAddress::addPortList(int newPortNumber, vector<int> portVector){
    portVector.emplace_back(newPortNumber);
    return;
}
/*void addPortList(int newPortNumber, vector<int> portVector);*/

// list ports being scanned
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

// check port number validity
NoobCodes ScanAddress::portValidityCheck(int portNumToCheck){
    if(portNumToCheck < 0 || portNumToCheck > 65535){
        return NoobCodes::portNumberInvalid;
    }
    return NoobCodes::success;
}
/*NoobCodes portValidityCheck(int portNumToCheck);*/

unsigned int ScanAddress::getSleepTimer(){
    return sleepTimer;
}
/*unsigned int getSleepTimer();*/

// get timeout wait time
unsigned int ScanAddress::getTimeoutTimer(){
    return this->timeOut;
}
/*unsigned int getTimeoutTimer();*/

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
                    
                    // to make things easier on return, topy readable mac to string
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

// get the number of times the port will be retried
int ScanAddress::getRetries(){
    return this->retries;
}
/*int getRetries();*/

string ScanAddress::getInterface(){
    return this->ourInterface;
}
/*string getInterface();*/

// variable delay status
bool ScanAddress::getVariableDelayStatus(){
    return this->variableDelay;
}
/*bool getVariableDelayStatus();*/

void ScanAddress::setCustomList(vector<unsigned int> newList){
    this->customList.clear();
    this->customList = newList;
}
/*void setCustomList(vector<unsigned> newList);*/

void ScanAddress::setSleepTimer(unsigned int newSleepTime){
    this->sleepTimer=newSleepTime;
    return;
}
/*void setSleepTimer(unsigned int newSleepTime);*/

void ScanAddress::setTimeoutTimer(unsigned int newTimeout){
    this->timeOut = newTimeout;
}
/*void setTimeoutTimer(unsigned int newTimeout);*/

// set the number of times the port will be retried
void ScanAddress::setRetries(int newRetryAmount){
    this->retries = newRetryAmount;
    return;
}
/*void setRetries(int newRetryAmount);*/

// set the user interface
void ScanAddress::setInterface(string ifType){
    
    // ensure any items in string are fully cleared to avoid oddities from arising
    this->ourInterface.clear();
    
    // assign new interface string
    this->ourInterface = ifType;
}
/*void setInterface(string ifType);*/

void ScanAddress::setVariableDelayStatus(bool variableDelayStatus){
    this->variableDelay = variableDelayStatus;
    return;
}
/*void setVariableDelayStatus(bool variableDelayStatus);*/

// training wheels scan to prompt user step by step
NoobCodes ScanAddress::assistedScan(){
    int userAnswer=0;
    int portToScan=1;
    string targetToScan;
    
    // ask user what kind of scan they want to do
    cout << "What kind of scan would you like to conduct?\n";
    cout << "\t1. TCP\n";
    cout << "\t2. UDP\n";
    
    userAnswer = getValidInput(1,2);
    
    // have user enter the IP address
    cout << "Enter IP address, or URL, that you are targeting for the scan:\n";
    getline(cin,targetToScan);
    
    // check IP validity
    
    
    // have user enter the port to scan
    cout << "Enter 1 port number to scan:\n";
    portToScan = getValidInput(1,65535);
    
    // run the scan
    
    return NoobCodes::fail;
}
/*NoobCodes assistedScan();*/

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

// set seed with random number
unsigned ScanAddress::generateNewSeed(){
    // pull the time since epoch, and then mod it by 10000 (variable time up to 1 second)
    unsigned seed = (unsigned) std::chrono::system_clock::now().time_since_epoch().count();
    return (seed%1000000);
}
/*unsigned generateNewSeed();*/

// translate NoobCode to char
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

void ScanAddress::debug(){
    return;
}
/*void debug();*/


