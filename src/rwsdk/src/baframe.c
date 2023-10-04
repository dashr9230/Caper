/*
 * Frame handling
 * Frames define relationships between objects and the world
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */

/**
 * \ingroup rwframe
 * \page rwframeoverview RwFrame Overview
 *
 * This object provides a frame of reference for other objects, allowing them to 
 * be positioned relative to each other in world space. In addition, a frame object 
 * can also be linked to parent and/or child frames in a hierarchical manner. Combined, 
 * these features make the frame among the most important RenderWare Graphics objects. It is 
 * used primarily in conjunction with RenderWare Graphics' Retained Mode API, exposed by the 
 * RpWorld Plugin.
 *
 * More information can be found in the \e Fundamentals chapter of the User Guide.
 *
*/

/****************************************************************************
 Includes
 */

#include "batypes.h"

#include "balibtyp.h"

#include "baframe.h"



/****************************************************************************
 Local Defines
 */

#define RWFRAMEGLOBAL(var) (RWPLUGINOFFSET(rwFrameGlobals, RwEngineInstance, frameModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    frameTKList =
    { sizeof(RwFrame),
      sizeof(RwFrame),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo frameModule;

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing library

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwFrameOpen

 On entry   :
 On exit    : TRUE on success
 */

void *
_rwFrameOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwFrameOpen"));

    /* Store offset of global data (sam for all instances) */
    frameModule.globalsOffset = offset;

    RWFRAMEGLOBAL(frames) = RwFreeListCreate(frameTKList.sizeOfStruct, 50,
                                             rwFRAMEALIGNMENT);
    if (!RWFRAMEGLOBAL(frames))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* Empty the dirty matrices */
    rwLinkListInitialize(&RWSRCGLOBAL(dirtyFrameList));

    /* One more module instance */
    frameModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwFrameClose

 On entry   :
 On exit    :
 */

void *
_rwFrameClose(void *instance,
              RwInt32 __RWUNUSED__ offset,
              RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwFrameClose"));

    if (RWFRAMEGLOBAL(frames))
    {
        RwFreeListDestroy(RWFRAMEGLOBAL(frames));
        RWFRAMEGLOBAL(frames) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    frameModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   cloning of frames

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */






