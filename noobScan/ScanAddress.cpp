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

// TODO: print host name in human readable format. Nothing is working ...
void ScanAddress::getHostIP(){
    
    // attempt 1
    /*
//    cout << "Variable setup\n";
//    char * send_buf[400];
//    char ipHolder[15];
//    struct ip *ourIP = (struct ip *)send_buf;
//    struct hostent *ourMachine;
//    char unreadableHostIP[256];
//    string readableHostIP;
//
//    cout << "gethostname\n";
//    gethostname(unreadableHostIP, sizeof(unreadableHostIP));
//    //printf("Host IP: %s\n", unreadableHostIP);
//    cout << "gethostbyname\n";
//    ourMachine=gethostbyname(unreadableHostIP);
//    cout << "transcribing IP\n";
//    ourIP->ip_src = (*(struct in_addr *)ourMachine->h_addr);
//    cout << "Printing IP.\n";
////    cout << inet_ntoa(ourIP->ip_src);
//    sprintf(ipHolder, "%s", inet_ntoa(ourIP->ip_src));
//    cout << ipHolder;
//    return;
     */
    
    //attempt 2
    /*
//    char send_buf[400], src_name[256], src_ip[15];
//    struct ip *ip = (struct ip *)send_buf;
//
//    struct hostent *src_hp;
//    int on;
//
//
//
//    //Initialize variables
//    on = 1;
//    memset(send_buf, 0, sizeof(send_buf));
//
//    // Get source IP address
//    if(gethostname(src_name, sizeof(src_name)) < 0)
//    {
//        perror("gethostname() error");
//        exit(EXIT_FAILURE);
//    }
//    else
//    {
//        if((src_hp = gethostbyname(src_name)) == NULL)
//        {
//            fprintf(stderr, "%s: Can't resolve, unknown source.\n", src_name);
//            exit(EXIT_FAILURE);
//        }
//        else
//            ip->ip_src = (*(struct in_addr *)src_hp->h_addr);
//    }
//
//
//    sprintf(src_ip, "%s", inet_ntoa(ip->ip_src));
//    printf("Source IP: '%s'\n", src_ip);
    */
    
    //attempt 3
    /*
//    char hostbuffer[256];
//    char *IPbuffer;
//    struct hostent *host_entry;
//    int hostname;
//
//    // To retrieve hostname
//    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
////    checkHostName(hostname);
//    if(hostname==-1){
//        cout << "HOSTNAME FUCKED UP\n";
//        return;
//    }
//
//    // To retrieve host information
//    host_entry = gethostbyname("localhost");
////    checkHostEntry(host_entry);
//    if(host_entry==NULL){
//        cout << "h_errno is: " << h_errno << endl;
//        switch(h_errno){
//            case HOST_NOT_FOUND:
//                cout << "Host not found\n";
//                break;
//            case TRY_AGAIN:
//                cout << "Try again\n";
//                break;
//            case NO_RECOVERY:
//                cout << "No recovery\n";
//                break;
//            case NO_DATA:
//                cout << "No data.";
//                break;
//            default:
//                cout << "some other issue.\n";
//                cout << errno << endl;
//                break;
//        }
//        return;
//    }
//
//    // To convert an Internet network
//    // address into ASCII string
//    IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
//
//    printf("Hostname: %s\n", hostbuffer);
//    printf("Host IP: %s", IPbuffer);
*/
    
    //attempt 4
    /*
//    struct ifaddrs *id;
//    int val;
//    val = getifaddrs(&id);
//    printf("Network Interface Name :- %s\n",id->ifa_name);
//    printf("Network Address of %s :- %d\n",id->ifa_name,(int*)id->ifa_addr);
//    printf("Network Data :- %d \n",(int)id->ifa_data);
//    printf("Socket Data : -%s\n",id->ifa_addr->sa_data);
//    return;
     */
    //attempt 5
    /*
    char host[256];
    char *IP;
    struct hostent *host_entry;
    int hostname;
    hostname = gethostname(host, sizeof(host)); //find the host name
    if(hostname==-1){
        cout << "hostname fucked you\n";
        return;
    }
    host_entry = gethostbyname("Scanme.Nmap.Org"); //find host information
    if(host_entry==NULL){
        cout << "fucked by host_entry\n";
        return;
    }
    IP = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); //Convert into IP string
    printf("Current Host Name: %s\n", host);
    printf("Host IP: %s\n", IP);
     */
    
    // attempt 6
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in *sa;
    char *addr;

    getifaddrs (&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family==AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            printf("Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
        }
    }

    freeifaddrs(ifap);
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
    
    // holder for the mac address
    unsigned char mac[6];
    
    // populate structures to hold mac address, transfer accordingly
    if (0 == getifaddrs(&if_addrs)) {
      for (if_addr = if_addrs; if_addr != NULL; if_addr = if_addr->ifa_next){
          if (strcmp(if_addr->ifa_name,"en0")==0 && if_addr->ifa_addr != NULL && if_addr->ifa_addr->sa_family == AF_LINK){
              struct sockaddr_dl* sdl = (struct sockaddr_dl *)if_addr->ifa_addr;
//              // holder for the mac address
//              unsigned char mac[6];
              if (6 == sdl->sdl_alen) {
                  memcpy(mac, LLADDR(sdl), sdl->sdl_alen);
                  // format MAC address appropriately. Note: %02x forces at least 2 digits to appear for each entry in the MAC address. This is for both formatting and the leading 0.
                  
                  printf("mac  : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                  
                  // for debug: see what the address looks like without format
                  cout << "Also: " << mac[0] << ":" << mac[1] << ":"  << mac[2] << ":" << mac[3] << ":"  << mac[4] << ":"  << mac[5] << endl;
              }
          }
          
      }
        freeifaddrs(if_addrs);
        if_addrs = NULL;
        
    }
    else {
        printf("getifaddrs() failed with errno =  %i %s\n", errno, strerror(errno));
        
    }
    return;
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

