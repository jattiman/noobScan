/**/
/*
   HelpModule.cpp of noobScan, a novice port scanner
   HelpModule.cpp provides information to the user about commands
*/
/**/

#include "HelpModule.h"

HelpModule::HelpModule(){
    return;
}

// displays opening prompt for the user to select info for
void HelpModule::displayDirections(){
    std::cout << "Type your command, or\n\t'help' for directions\n\t'settings' to adjust settings\n\t'exit' to exit\n";
    return;
}

// tells the user everything they can say
void HelpModule::displayOptions(){
    std::cout << "Detailed commands: " << std::endl;
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(std::string userString){
    
    return;
}
