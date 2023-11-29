/*
 * Raster handling
 * Rasters are device specific representations of bitmaps
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 *
 */

/**
 * \ingroup rwraster
 * \page rwrasteroverview RwRaster Overview
 *
 * The Raster is a platform-dependent representation of bitmaps.
 * Raster formats are largely dictated by the underlying hardware and
 * are not even guaranteed to use the RGB color model - some platforms
 * may even use compressed formats.
 *
 * Rasters need to be locked if you need to access or edit their contents
 * and should be unlocked after processing.
 *
 * It is possible to tell RenderWare Graphics that you only wish to read
 * data, or access structures like palettes, so that unlocking does not
 * impact too much on performance.
 *
 * These hints are used to determine whether RenderWare needs to perform
 * conversions on the data before and/or after unlocking.
 *
 * Rasters are closely connected with \see RwCamera, \see RwImage and
 * \see RwTexture objects.
 *
 */

/***************************************************************************/

/* NOTE: NOTHING IN THIS FILE SHOULD DEPEND ON RWIMAGE
 * - THAT'S WHAT BAIMRAS.C/H IS FOR */

/***************************************************************************/

/****************************************************************************
 Includes
 */



#include "batypes.h"
#include "balibtyp.h"

#include "batkreg.h"


/* Raster handling */
#include "baraster.h"




/****************************************************************************
 Local Defines
 */

#define RWRASTERGLOBAL(var)                             \
    (RWPLUGINOFFSET(rwRasterGlobals,                    \
                    RwEngineInstance,                   \
                    rasterModule.globalsOffset)->var)





/****************************************************************************
 Local (static) Globals
 */

static RwPluginRegistry rasterTKList = 
 { sizeof(RwRaster),
    sizeof(RwRaster),
    0,
    0,
    (RwPluginRegEntry *) NULL,
    (RwPluginRegEntry *) NULL
 };

static RwModuleInfo rasterModule;
















/**
 * \ingroup rwraster
 * \ref RwRasterDestroy is used to destroy the specified raster and free
 * any resources it may have used.
 *
 * \param raster  Pointer to the raster.
 *
 * \return Returns TRUE if successful or FALSE if there is an error.
 *
 * \see RwRasterCreate
 *
 */
#include <windows.h>
RwBool
RwRasterDestroy(RwRaster * raster)
{
    RWAPIFUNCTION(RWSTRING("RwRasterDestroy"));
    RWASSERT(rasterModule.numInstances);
    RWASSERT(raster);

    /* De-initialize the plugin data */
    _rwPluginRegistryDeInitObject(&rasterTKList, raster);

    /* TODO: RwRasterDestroy, when stdFunc added */

    /* Blow it away */
    RwFreeListFree(RWRASTERGLOBAL(rasterFreeList), raster);

    /* All done */
    RWRETURN(TRUE);
}





/**
 * \ingroup rwraster
 * \ref RwRasterShowRaster is used to copy the specified raster to the
 * display device. The raster must be of type rwRASTERTYPECAMERA.
 *
 * \param raster  Pointer to the raster.
 * \param dev  A device-dependent parameter, e.g. for Windows applications the
 *       handle of the output window returned by CreateWindow (type HWND).
 * \param flags  A \ref RwUInt32 bit-field value equal to the raster display options
 *       (type \ref RwRasterFlipMode):
 *
 *       \li rwRASTERFLIPWAITVSYNC - Wait for the next vertical retrace on
 *           the output device, if possible, before displaying (full-screen
 *           applications only). Specify zero otherwise.
 *
 * \return Returns pointer to the raster if successful or NULL if there
 *        is an error.
 *
 * \see RwCameraShowRaster
 * \see RwRasterCreate
 * \see RwRasterGetFormat
 *
 */
RwRaster           *
RwRasterShowRaster(RwRaster * raster, void *dev, RwUInt32 flags)
{
    // TODO: RwRasterShowRaster

    /* Device failure */
    RWRETURN((RwRaster *) NULL);
}


















/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening/Closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwRasterClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwRasterClose(void *instance,
               RwInt32 __RWUNUSED__ offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwRasterClose"));

    if (RWRASTERGLOBAL(rasterFreeList))
    {
        RwFreeListDestroy(RWRASTERGLOBAL(rasterFreeList));
        RWRASTERGLOBAL(rasterFreeList) = (RwFreeList *) NULL;
    }

    /* One less module instance */
    rasterModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwRasterOpen

 On entry   :
 One exit   : TRUE on success
 */

void               *
_rwRasterOpen(void *instance, RwInt32 offset, RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwRasterOpen"));

    /* Save the globals offset (same for all instances) */
    rasterModule.globalsOffset = offset;

    /* And get on with the setup */
    memset(&RWRASTERGLOBAL(dummyRaster), 0,
           sizeof(RWRASTERGLOBAL(dummyRaster)));
    RWRASTERGLOBAL(dummyRaster).width = 0;
    RWRASTERGLOBAL(dummyRaster).height = 0;
    RWRASTERGLOBAL(dummyRaster).depth = 0;
    RWRASTERGLOBAL(dummyRaster).cFlags = (RwUInt8) rwRASTERDONTALLOCATE;
    RWRASTERGLOBAL(dummyRaster).cpPixels = (unsigned char *) NULL;
    RWRASTERGLOBAL(dummyRaster).palette = (unsigned char *) NULL;
    RWRASTERGLOBAL(dummyRaster).cType = (RwUInt8) rwRASTERTYPENORMAL;

    RWRASTERGLOBAL(rasterSP) = 0;
    RWRASTERGLOBAL(rasterStack[0]) = &RWRASTERGLOBAL(dummyRaster);

    RWRASTERGLOBAL(rasterFreeList) =
        RwFreeListCreate(rasterTKList.sizeOfStruct, 20, 0);
    if (!RWRASTERGLOBAL(rasterFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    rasterModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}
