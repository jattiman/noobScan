//
//  NoobCodes.h
//  noobScan
//
//  Created by John Atti on 4/14/20.
//  Copyright © 2020 John Atti. All rights reserved.
//

#ifndef NoobCodes_h
#define NoobCodes_h

enum class NoobCodes{
    fail,
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
