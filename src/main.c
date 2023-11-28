
/* temporary including from rwsdk. TODO: remove this */
#include <ostypes.h> /* rwplcore.h */
#include <batypes.h> /* rwplcore.h */

#include "skeleton.h"

#define DEFAULT_SCREEN_WIDTH (640)
#define DEFAULT_SCREEN_HEIGHT (480)

static RwUInt32 lastFrameTime = 0;




/*
 *****************************************************************************
 */
static RwBool
Initialize(void)
{
    if( RsInitialize() )
    {
        if( !RsGlobal.maximumWidth && !RsGlobal.maximumHeight )
        {
            RsGlobal.maximumWidth = DEFAULT_SCREEN_WIDTH;
            RsGlobal.maximumHeight = DEFAULT_SCREEN_HEIGHT;
        }

        lastFrameTime = RsTimer();

        return TRUE;
    }

    return FALSE;
}




/*
 *****************************************************************************
 */
static RwBool
Initialize3D(void *param)
{
    if( !RsRwInitialize(param) )
    {
        RsErrorMessage(RWSTRING("Error initializing RenderWare."));

        return FALSE;
    }


    /* TODO: Initialize3D */

}







/*
 *****************************************************************************
 */
static RwBool
AttachPlugins(void)
{
    /* TODO: AttachPlugins */
}








/*
 *****************************************************************************
 */
RsEventStatus
AppEventHandler(RsEvent event, void *param)
{
	switch( event )
    {
		case rsINITIALIZE:
        {
            return Initialize() ? rsEVENTPROCESSED : rsEVENTERROR;
        }

        case rsRWINITIALIZE:
        {
            return Initialize3D(param) ? rsEVENTPROCESSED : rsEVENTERROR;
        }



        case rsPLUGINATTACH:
        {
            return AttachPlugins() ? rsEVENTPROCESSED : rsEVENTERROR;
        }


	}
}
