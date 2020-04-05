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
    std::cout << "Type 'help' for directions, 'settings' to adjust settings, 'exit' to exit, or simply enter your scan command.\n";
    return;
}

// returns help results based on the user request
void HelpModule::returnInfo(std::string userString){
    return;
}
