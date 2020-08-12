/**/
/*
   main.cpp of noobScan, a novice port scanner
   main.cpp welcomes the user and runs scan options
*/
/**/

#include "NoobScan.h"

using namespace std;

void parseUserArgument(string userCommand){
    
    userCommand.clear();
    cout << "Enter command: ";
    getline(cin,userCommand);
    
    vector<string> parsedCommand;
    vector<int> portsToScan;
    vector<string> ipToScan;
    
    
    
    // ensure any previous commands are empty
    //cout << "setting up regex\n";
    // set up regex to match commands with what we're looking for
    smatch matches;
    smatch matches2;
    smatch matches3;
    
    // this searches for words only (no numbers/white space)
    regex commandHunter("\\b[^\\d\\W]+\\b");
    
    // this searches for numbers only (it's how we identify ports)
    //regex portHunter("\\b[0-9]{1,}");
    //regex portHunter("\\b[0-9]+[^.\\s]");
    //regex portHunter("[^\\.]\\b[0-9]+\\b(?!\\.)");
    regex portHunter("[^\\.]\\b[0-9]+\\b(?!\\.)");
    //regex portHunter("[^\\.]\\b[0-9]+\\b(?!\\.)"); \\2 slashes aren't needed before . in []
    //regex portHunter("\\b(?=\\w)\\d+\\b(?<=\\w)");
    //regex portHunter("^0$|[1-9][0-9]*$");
    //regex portHunter("[^\\.\\D][\\d]{1,}[^\\.]"); // works but gets IP too
    
    // This searches for our IP address, if it's there (if not, we assume a URL is being used)
    regex ipHunter(R"(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})");
    //regex ipHunter("\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}"); // USE THIS
    
    //regex ipHunter("\\d{1,3}.\\d{1,3}.\\d{1,3}.\\d{1,3}");
    //regex ipHunter("([0-9]{1,3}[.])");
    
    
    //cout << "copying string.\n";
    // save the userCommand string, because we're about to demolish it
    string passOne = userCommand;
    // pass 2 for port hunting
    string passTwo = userCommand;
    // pass 3 for ip address hunting
    string passThree = userCommand;
    
    //cout << "first pass\n";
    // first pass: check for word-only commands
    while(regex_search(passOne, matches, commandHunter)){
        // for all matches
        for(auto i:matches){
            // once a match is found, push it back
            parsedCommand.push_back(i);
            // trim the found match from the string being searched
            passOne=matches.suffix().str();
        }
    }
    
    //cout << "2nd pass\n";
    // second pass: check for number-only commands (ports)
    bool firstError=true;
    while(regex_search(passTwo, matches2, portHunter)){
        // for all matches
        
        for(auto i:matches2){
            // once a match is found, push it to the port list (remember to convert to an int, so your program doesn't implode)
            try {
                portsToScan.push_back(stoi(i));
            } catch (const std::invalid_argument) {
                if(firstError){
                    portsToScan.pop_back();
                    firstError=false;
                }
                cout << "Your formatting is off (misreading for " << i << "). Results may be unexpected.\n";
            }
            // trim the found match from the string, and continue searching for matches
            passTwo=matches2.suffix().str();
        }
    }
    
    //cout << "3rd pass\n";
    // third pass: check for IP addresses
    while(regex_search(passThree, matches3, ipHunter)){
        // for all matches
        for(auto i:matches3){
            cout << "Found match\n";
            ipToScan.push_back(i);
            passThree=matches3.suffix().str();
        }
    }
    
    cout << "Commands:\n";
    for(auto &i:parsedCommand){
        cout << "\t" << i << endl;
    }
    
    cout << "Ports:\n";
    for(auto &i:portsToScan){
        cout << "\t" <<i << endl;
    }
    
    
    cout << "IP addresses:\n";
    for(auto &i:ipToScan){
        cout << "\t" <<i << endl;
    }
    userCommand.clear();
    parsedCommand.clear();
    portsToScan.clear();
    ipToScan.clear();
    return;
}

void debug(){
    //    if( getuid() !=0){
    //        cout << "Not root.\n";
    //    }
    //    else{
    //        cout << "Root\n";
    //    }
    while(true){
        parseUserArgument("dummy");
    }
//        cout << "Pass 1: normal IP:\n";
//        parseUserArgument("help scan 66 192.168.1.1");
//
//        cout << "Pass 2: IP + ports:\n";
//        parseUserArgument("help scan 192.168.1.1 11 22 33 44");
//
//        cout << "Pass 3: ports + IP:\n";
//        parseUserArgument("help scan 99 88 77 66 192.168.1.1");
//
//        cout << "Pass 4: normal IP:\n";
//        parseUserArgument("help scan 77 227.0.0.1");
}


int main(int argc, const char * argv[]) {
    
    NoobScan ourNoob;
//    ourNoob.initialPrompt();
    
    ourNoob.debug();
//    debug();
    

    return 0;
}
