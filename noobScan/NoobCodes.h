/**/
/*
 NoobCodes.h of NoobScan, a novice portscanner
 NoobCodes.h contains the enum class essential for communicating throughout the NoobScan program. 
 Author: John Atti
 Finished 8/16/2020
*/
/**/

#ifndef NoobCodes_h
#define NoobCodes_h

enum class NoobCodes{
    fail,
    failPartial,
    failURL,
    failIP,
    failPort,
    success,
    restart,
    tcp,
    udp,
    fin,
    syn,
    helpRequest,
    scanRequest,
    exitRequest,
    debugRequest,
    IPRequest,
    settingsRequest,
    settingsForDelay,
    settingsForDictionary,
    settingsForAssistedScan,
    settingsForRecorder,
    settingsForScanGroups,
    settingsForTimeouts,
    settingsForDebugText,
    socketError,
    socketCreationError,
    socketCreationErrorICMP,
    socketCreationErrorDGRAM,
    socketCreationErrorSTREAM,
    portConnectionError,
    portConnectionDenied,
    portConnectionSuccess,
    portNumberInvalid,
    portSendError,
    portSendDenied,
    portReceiveError,
    IPBindingIssue,
    hostNameIssue
};

#endif /* NoobCodes_h */
