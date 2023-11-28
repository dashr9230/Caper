
/* temporary including from rwsdk. TODO: remove this */
#include <ostypes.h> /* rwplcore.h */
#include <batypes.h> /* rwplcore.h */
#include <bamemory.h>
#include <badevice.h> /* rwplcore.h */


#include "skeleton.h"
#include "platform.h"

RsGlobalType                RsGlobal;


/*
 *****************************************************************************
 */
RwUInt32
RsTimer(void)
{
    return psTimer();
}



/*
 *****************************************************************************
 */
void
RsErrorMessage(const RwChar * text)
{
    RsKeyStatus         ks;

    /*  KEYUP message lost on dialogue box -
     *  send NULL message to prevent multiple dbox instances
     */

    ks.keyScanCode = rsNULL;
    ks.keyCharCode = rsNULL;

    RsKeyboardEventHandler(rsKEYUP, &ks);

    psErrorMessage(text);

	return;
}

/*
 *****************************************************************************
 */
void
RsWarningMessage(const RwChar * text)
{
    RsKeyStatus         ks;

    /*  KEYUP message lost on dialogue box -
     *  send NULL message to prevent multiple dbox instances
     */

    ks.keyScanCode = rsNULL;
    ks.keyCharCode = rsNULL;

    RsKeyboardEventHandler(rsKEYUP, &ks);

    psWarningMessage(text);

    return;
}







/*
 *****************************************************************************
 */
RwBool
RsAlwaysOnTop(RwBool alwaysOnTop)
{
    return psAlwaysOnTop(alwaysOnTop);
}







/*
 *****************************************************************************
 */
static              RwBool
RsSetDebug(void)
{
	return TRUE;
}

/*
 *****************************************************************************
 */
void
RsMouseSetVisibility(RwBool visible)
{
    psMouseSetVisibility(visible);

    return;
}

/*
 *****************************************************************************
 */
void
RsMouseSetPos(RwV2d * pos)
{
    psMouseSetPos(pos);

    return;
}













/*
 *****************************************************************************
 */
RsEventStatus
RsKeyboardEventHandler(RsEvent event, void *param)
{
    if (RsGlobal.keyboard.used)
    {
        return RsGlobal.keyboard.inputEventHandler(event, param);
    }

    return rsEVENTNOTPROCESSED;
}








/*
 *****************************************************************************
 */

RsEventStatus
RsEventHandler(RsEvent event, void *param)
{
	RsEventStatus       result;
	RsEventStatus       es;

	/*
     * Give the application an opportunity to override any events...
     */
    es = AppEventHandler(event, param);

	/*
     * We never allow the app to replace the quit behaviour,
     * only to intercept...
     */
    if (event == rsQUITAPP)
    {
        /*
         * Set the flag which causes the event loop to exit...
         */
        RsGlobal.quit = TRUE;
    }

	if (es == rsEVENTNOTPROCESSED)
    {
		// TODO: Temporary, remove this
		result = rsEVENTPROCESSED;

		switch (event)
		{

			case rsINITDEBUG:
                result =
                    (RsSetDebug()? rsEVENTPROCESSED : rsEVENTERROR);
                break;



			case rsTERMINATE:
                RsTerminate();
                result = (rsEVENTPROCESSED);
                break;

			case rsINITIALIZE:
                result =
                    (RsInitialize()? rsEVENTPROCESSED : rsEVENTERROR);
                break;


			


			default:
                result = (es);
                break;

		}
	}
	else
	{
		result = (es);
	}

	return result;
}






/*
 *****************************************************************************
 */
RwBool
RsRwInitialize(void *displayID)
{
    /* 
     * Start RenderWare...
     */
    if (!RwEngineInit(psGetMemoryFunctions(), 0, rsRESOURCESDEFAULTARENASIZE))
    {
        return (FALSE);
    }

	/* 
     * Install any platform specific file systems...
     */
    psInstallFileSystem();

	/* 
     * Initialize debug message handling...
     */
    RsEventHandler(rsINITDEBUG, NULL);

	/* 
     * Attach all plugins...
     */
    if (RsEventHandler(rsPLUGINATTACH, NULL) == rsEVENTERROR)
    {
        return (FALSE);
    }
}




/*
 *****************************************************************************
 */
void
RsTerminate(void)
{
    psTerminate();

    return;
}

/*
 *****************************************************************************
 */
RwBool
RsInitialize(void)
{
	/*
	 * Initialize Platform independent data...
	 */
	RwBool              result;

	RsGlobal.appName = RWSTRING("The Italian Job");
	RsGlobal.maximumWidth = 0;
	RsGlobal.maximumHeight = 0;
	RsGlobal.quit = FALSE;

	/* setup the keyboard */
	RsGlobal.keyboard.inputDeviceType = rsKEYBOARD;
	RsGlobal.keyboard.inputEventHandler = 0;
	RsGlobal.keyboard.used = 0;

	/* setup the mouse */
	RsGlobal.mouse.inputDeviceType = rsMOUSE;
	RsGlobal.mouse.inputEventHandler = 0;
	RsGlobal.mouse.used = 0;

	/* setup the pad */
	RsGlobal.pad.inputDeviceType = rsPAD;
	RsGlobal.pad.inputEventHandler = 0;
	RsGlobal.pad.used = 0;

	result = psInitialize();

	return result;
}













