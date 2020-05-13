/**/
/*
   scanaddress.cpp of noobScan, a novice port scanner
   scanaddress.cpp
*/
/**/

#include "ScanAddress.h"

ScanAddress::ScanAddress(){
    this->sleepTimer=0;
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
struct hostent * ScanAddress::returnHostIP(std::string targetHost){
        
    // create host structure to convert URL to IP address
    struct hostent *ourHost;
    
    // convert to IP accordingly
    ourHost=gethostbyname(targetHost.c_str());
        
    // if issue with conversion, return NULL
    if(ourHost == NULL){
        return NULL;
    }
    else{
        return ourHost;
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

void ScanAddress::getHostMac(){
    
    // define structures
    struct ifaddrs *if_addrs = NULL;
    struct ifaddrs *if_addr = NULL;
    
    // populate structures to hold mac address, transfer accordingly
    if (0 == getifaddrs(&if_addrs)) {
      for (if_addr = if_addrs; if_addr != NULL; if_addr = if_addr->ifa_next){
          if (strcmp(if_addr->ifa_name,"en0")==0 && if_addr->ifa_addr != NULL && if_addr->ifa_addr->sa_family == AF_LINK){
              struct sockaddr_dl* sdl = (struct sockaddr_dl *)if_addr->ifa_addr;
              // Why 6? Digital high five if you enter in the chat before I explain
              unsigned char mac[6];
              if (6 == sdl->sdl_alen) {
                  memcpy(mac, LLADDR(sdl), sdl->sdl_alen);
                  // format mac address appropriately
                  printf("mac  : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                  // for debug: see what the address looks like without format
                  cout << "Also: " << mac[0] << mac[1] << mac[2] << mac[3] << mac[4] << mac[5] << endl;
              }
          }
          
      }
        freeifaddrs(if_addrs);
        if_addrs = NULL;
        
    }
    else {
        printf("getifaddrs() failed with errno =  %i %s\n", errno, strerror(errno));
        
    }
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

