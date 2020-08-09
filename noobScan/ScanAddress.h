/**/
/*
   Scanaddress.h of noobScan, a novice port scanner
   Scanaddress.h
 Controls the settings of the scanner
*/
/**/

#ifndef ScanAddress_h
#define ScanAddress_h

//#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//for IP address
#include <netinet/ip.h>

// for ICMP usage - must include after ip.h to avoid conflicts
#include <netinet/ip_icmp.h>

// to pull MAC address
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <ifaddrs.h>
#include <errno.h>
#include <stdio.h>

#include <string>
#include <iostream>
#include <vector>

#include "NoobCodes.h"


using namespace std;

class ScanAddress{
    
public:
    
    ScanAddress();
    
    // confirm user input is valid
    int getValidInput(int minNum=0, int maxNum=65535);
    
    // confirm the scan type being requested
    char checkScanType(string scanCommand);
    
    // initiate scan, report results and output to screen
    string initiateScan(char ourScanType);
    
    // resolve target IP from URL
    string getTargetIP(string targetHost);
    
    // print host name in human readable format
    string getHostIP(string ifaNamePreference = "en0");
    
    // add to port list
    void addPortList(int newPortNumber, vector<int> portVector);
    
    // list ports being scanned
    void getPortList();
    
    // check port number validity
    NoobCodes portValidityCheck(int portNumToCheck);//0 and 65535
    
    // get sleep time
    unsigned int getSleepTimer();
    
    // get host MAC address
    string getHostMac(string ifaNamePreference = "en0");
    
    // get port retry number
    int getRetries();
        
    // get the user interface
    string getInterface();
    
    // variable delay status
    bool getVariableDelayStatus();
    
    // set custom port list vector
    void setCustomList(vector<unsigned> newList);
    
    // set sleep time - the time between retries (how long you wait for a timeout)
    void setSleepTimer(unsigned int newSleepTime);
    
    // set the retry number
    void setRetries(int newRetryAmount);
    
    // set the user interface
    void setInterface(string ifType);
    
    // set variable delay
    void setVariableDelayStatus(bool variableDelayStatus);
    
    // training wheels scan to prompt user step by step
    NoobCodes assistedScan();
    
    // print debug statement
    void debug();
    
    
private:
    
    // array of ports
    unsigned short *portList;
    
    // code for scan type (tcp/udp)
    char scanType;
    
    // time out time
    int timeOut;
    
    // save to file
    bool saveToFile;
    
    // output to screen
    bool screenOutput;
    
    // variable delay between scans
    bool variableDelay;
    
    // vector of ports to scan (defined by user)
    vector<unsigned> portsToScan;
    
    // vector of popular ports to scan (predefined)
    vector<unsigned> popularPorts {21, 22, 23, 25, 53, 80, 110, 111, 135, 139, 143, 443, 445, 993, 995, 1723, 3306, 3389, 5900, 8080};
    
    // vector of chat-type ports to scan (predefined)
    vector<unsigned> chatPorts {1863, 5050, 5190, 5222, 5223, 6665, 6666, 6667, 6668, 6669, 6679, 6697, 6891, 6892, 6893, 6894, 6895, 6896, 6897, 6898, 6899, 6900, 6901, 7648, 7649, 8767, 9119, 25999};
    
    // vector of typical gaming-type ports to scan (predefined)
    vector<unsigned> gamingPorts {1725, 2302, 3074, 3724, 6112, 6500, 12035, 12036, 14567, 15118, 27015, 28960};
    
    // vector of typical P2P (peer to peer) ports to scan (predefined)
    vector<unsigned> peerPorts {411, 412, 1214, 1337, 4672, 6257, 6346, 6347, 6699, 6881, 6999};
    
    // vector of typical streaming ports to scan (predefined)
    vector<unsigned> streamingPorts {1755, 3784, 3785, 5001, 5004, 5005, 5060, 6970, 8000, 24800};
    
    // vector of historical malicious ports to scan (predefined)
    vector<unsigned> maliciousPorts {1080, 2745, 3127, 4444, 5554, 8866, 9898, 9988, 12345, 27374, 31337};
    
    // vector holding custom port list
    vector<unsigned> customList;
    
    // sleep timer
    unsigned int sleepTimer;
    
    // interface the use
    string ourInterface;
    
    // our mac address
    char ourMac[32]={0};
    
    // number of times to retry certain ports if there's a connection error
    int retries;
    
};

#endif /* ScanAddress_h */
