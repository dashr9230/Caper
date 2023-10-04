
#ifndef SKELETON_H
#define SKELETON_H



/* Default arena size depending on platform. */

#define rsRESOURCESDEFAULTARENASIZE (1 << 20) /* whatever the fuck this is */







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
	/* TODO: RsKeyCodes */

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
