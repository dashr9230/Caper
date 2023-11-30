
#ifndef SKELETON_H
#define SKELETON_H

#include "rwcore.h"

/* Default arena size depending on platform. */

#define rsRESOURCESDEFAULTARENASIZE (1 << 20)

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

enum RsInputDeviceType
{
    rsKEYBOARD,
    rsMOUSE,
    rsPAD
};
typedef enum RsInputDeviceType RsInputDeviceType;

enum RsEventStatus
{
    rsEVENTERROR,
    rsEVENTPROCESSED,
    rsEVENTNOTPROCESSED
};
typedef enum RsEventStatus RsEventStatus;

enum RsEvent
{
    rsCAMERASIZE,
    rsCOMMANDLINE,
    rsFILELOAD,
    rsINITDEBUG,
    rsINPUTDEVICEATTACH,
    rsLEFTBUTTONDOWN,
    rsLEFTBUTTONUP,
    rsMOUSEMOVE,
    rsPLUGINATTACH,
    rsREGISTERIMAGELOADER,
    rsRIGHTBUTTONDOWN,
    rsRIGHTBUTTONUP,
    rsRWINITIALIZE,
    rsRWTERMINATE,
    rsSELECTDEVICE,
    rsINITIALIZE,
    rsTERMINATE,
    rsIDLE,
    rsKEYDOWN,
    rsKEYUP,
    rsQUITAPP,
    RsEvent_Unk_21, /* TODO: Find out RsEvent_Unk_21 */
    RsEvent_Unk_22, /* TODO: Find out RsEvent_Unk_22 */
    RsEvent_Unk_23, /* TODO: Find out RsEvent_Unk_23 */
    RsEvent_Unk_24, /* TODO: Find out RsEvent_Unk_24 */
    rsPREINITCOMMANDLINE,
    rsACTIVATE,

    // TODO: RsEvent
};
typedef enum RsEvent RsEvent;

typedef RsEventStatus (*RsInputEventHandler)(RsEvent event, void *param);

typedef struct RsInputDevice RsInputDevice;
struct RsInputDevice
{
    RsInputDeviceType inputDeviceType;
    RwBool used;
    RsInputEventHandler inputEventHandler;
};

typedef struct RsGlobalType RsGlobalType;
struct RsGlobalType
{
    const RwChar *appName;
    RwInt32 maximumWidth;
    RwInt32 maximumHeight;
    RwBool  quit;

    void   *ps; /* platform specific data */

    RsInputDevice keyboard;
    RsInputDevice mouse;
    RsInputDevice pad;
};

enum RsKeyCodes
{
    rsESC       = 128,

    rsF1        = 129,
    rsF2        = 130,
    rsF3        = 131,
    rsF4        = 132,
    rsF5        = 133,
    rsF6        = 134,
    rsF7        = 135,
    rsF8        = 136,
    rsF9        = 137,
    rsF10       = 138,
    rsF11       = 139,
    rsF12       = 140,

    rsINS       = 141,
    rsDEL       = 142,
    rsHOME      = 143,
    rsEND       = 144,
    rsPGUP      = 145,
    rsPGDN      = 146,

    rsUP        = 147,
    rsDOWN      = 148,
    rsLEFT      = 149,
    rsRIGHT     = 150,

    rsPADINS    = 151,
    rsPADDEL    = 152,
    rsPADHOME   = 153,
    rsPADEND    = 154,
    rsPADPGUP   = 155,
    rsPADPGDN   = 156,

    rsPADUP     = 157,
    rsPADDOWN   = 158,
    rsPADLEFT   = 159,
    rsPADRIGHT  = 160,

    rsNUMLOCK   = 161,
    rsDIVIDE    = 162,
    rsTIMES     = 163,
    rsMINUS     = 164,
    rsPLUS      = 165,
    rsPADENTER  = 166,
    rsPAD5      = 167,

    rsBACKSP    = 168,
    rsTAB       = 169,
    rsCAPSLK    = 170,
    rsENTER     = 171,
    rsLSHIFT    = 172,
    rsRSHIFT    = 173,
    rsLCTRL     = 174,
    rsRCTRL     = 175,
    rsLALT      = 176,
    rsRALT      = 177,

    rsNULL       = 255
};
typedef enum RsKeyCodes RsKeyCodes;

typedef struct RsKeyStatus RsKeyStatus;
struct RsKeyStatus
{
    RwInt32     keyScanCode;
    RwInt32     keyCharCode;
};



extern RsGlobalType RsGlobal;

extern RsEventStatus AppEventHandler(RsEvent event, void *param);

extern RsEventStatus RsEventHandler(RsEvent event, void *param);
extern RsEventStatus RsKeyboardEventHandler(RsEvent event, void *param);

extern RwBool
RsInitialize(void);

extern RwBool                   
RsRwInitialize(void *param);



extern RwBool                   
RsAlwaysOnTop(RwBool alwaysOnTop);



extern RwUInt32                 
RsTimer(void);



extern void                     
RsErrorMessage(const RwChar *text);

extern void                     
RsMouseSetVisibility(RwBool visible);

extern void                     
RsMouseSetPos(RwV2d *pos);




extern void                     
RsTerminate(void);

extern void                     
RsWarningMessage(const RwChar *text);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#endif /* SKELETON_H */
