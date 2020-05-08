/**/
/*
   scanaddress.cpp of noobScan, a novice port scanner
   scanaddress.cpp
*/
/**/

#include "ScanAddress.h"


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
    //TODO: add try catch block to this
        
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
