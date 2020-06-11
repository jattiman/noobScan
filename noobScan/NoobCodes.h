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
    success,
    tcp,
    udp,
    fin,
    syn,
    helpRequest,
    scanRequest,
    exitRequest,
    debugRequest,
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
    portConnectionError,
    portConnectionDenied,
    portConnectionSuccess,
    portNumberInvalid,
    IPBindingIssue,
    hostNameIssue
};


#endif /* NoobCodes_h */
