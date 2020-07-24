/**/
/*
   scanaddress.cpp of noobScan, a novice port scanner
   scanaddress.cpp
*/
/**/

#include "ScanAddress.h"

ScanAddress::ScanAddress(){
    this->sleepTimer=2;
    this->retries=3;
}

// confirm the scan type being requested
char ScanAddress::checkScanType(std::string scanCommand){
    return 'a';
}

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

// resolve host name to IP address
string ScanAddress::getTargetIP(string targetHost){
    
    //TODO: error check URL here, or trust user
        
    // create host structure to convert URL to IP address
    struct hostent *ourHost;
    string targetIPAddress;
    
    // convert (store) the URL accordingly
    ourHost=gethostbyname(targetHost.c_str());
        
    // if issue with conversion, return NULL
    if(ourHost == NULL){
        //cout << "Null address.\n";
        return NULL;
    }
    // otherwise, return the IP address
    else{
        targetIPAddress = inet_ntoa(*((struct in_addr*) ourHost->h_addr_list[0]));
        return targetIPAddress;
    }
}

// TODO: print host IP in human readable format. Nothing is working ...
string ScanAddress::getHostIP(string ifaNamePreference){
    // create address structure for IP
    struct ifaddrs *ifaHolder;
    struct ifaddrs *ifaInspector;
    struct sockaddr_in *sa;
    string addr;
    string ourHostIP;

    // attempt to get list of machine's network interfaces

    // if this fails, abandon and report
    if(getifaddrs (&ifaHolder)==-1){
        cout << "Error finding network interfaces.\n";
        return {};
    }
    // if it succeeds, find the IP for the preferred interface
    else{
        // look through all interfaces on machine
        for (ifaInspector = ifaHolder; ifaInspector; ifaInspector = ifaInspector->ifa_next) {
            // ensure that the interface is
            if (ifaInspector->ifa_addr && ifaInspector->ifa_addr->sa_family==AF_INET) {
                sa = (struct sockaddr_in *) ifaInspector->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                if(ifaInspector->ifa_name == ifaNamePreference){
                    ourHostIP=addr;
                }

            }
        }
        freeifaddrs(ifaHolder);
        return ourHostIP;
    }
}

void ScanAddress::addPortList(int newPortNumber, vector<int> portVector){
    portVector.emplace_back(newPortNumber);
    return;
}

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

// check port number validity
NoobCodes ScanAddress::portValidityCheck(int portNumToCheck){
    if(portNumToCheck < 0 || portNumToCheck > 65535){
        return NoobCodes::portNumberInvalid;
    }
    return NoobCodes::success;
}

useconds_t ScanAddress::getSleepTimer(){
    return this->sleepTimer;
}

void ScanAddress::setSleepTimer(useconds_t newSleepTime){
    this->sleepTimer=newSleepTime;
    return;
}

string ScanAddress::getHostMac(string ifaNamePreference){
    
    // define structures
    struct ifaddrs *ifaHolder = NULL;
    struct ifaddrs *ifaInspector = NULL;
    
    // holder for the raw mac address
    unsigned char rawMac[6];
    
    // holder for the human readable mac address
    char ourMac[32]={0};
    
    // string to be returned
    string finalMac;
    
    // populate structures to hold mac address, transfer accordingly. Note: ioctl will not work on macOS as it would on LINUX, due to SIOCGIFHWADDR not being recognized on macOS currently. This is why we use this longer for loop to match the MAC with the interface being used, below.
    if (getifaddrs(&ifaHolder) == 0) {
        
        // loop through interfaces
        for (ifaInspector = ifaHolder; ifaInspector != NULL; ifaInspector = ifaInspector->ifa_next){
            
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
        freeifaddrs(ifaHolder);
        return finalMac;
    }
    
    // if getifaddrs fails, we return an empty string
    else {
        printf("Error: %s\n", strerror(errno));
        cout << "Error accessing host machine network interface.\n";
        return {};
    }
}

// get the number of times the port will be retried
int ScanAddress::getRetries(){
    return this->retries;
}

// set the number of times the port will be retried
void ScanAddress::setRetries(int newRetryAmount){
    this->retries=newRetryAmount;
    return;
}

void ScanAddress::debug(){

    ifaddrs* iflist;
    if (getifaddrs(&iflist) == 0) {
        for (ifaddrs* cur = iflist; cur; cur = cur->ifa_next) {
            if ((cur->ifa_addr->sa_family == AF_LINK) && (strcmp(cur->ifa_name, "en0") == 0) && cur->ifa_addr) {
                sockaddr_dl* sdl = (sockaddr_dl*)cur->ifa_addr;
                memcpy(this->ourMac, LLADDR(sdl), sdl->sdl_alen);
                break;
            }
        }
        freeifaddrs(iflist);
        
    }
    cout << "Mac: " << this->ourMac << endl;
    
}

