/*
 * Cameras define how and what things can be seen. They also define the
 * depth and width of the view by the use of clip planes and the view
 * window.
 */

/**
 * \ingroup rwcamera
 * \page rwcameraoverview RwCamera Overview
 *
 * RenderWare Graphics utilizes a standard virtual camera model and this object represents 
 * the virtual camera. Usually, a camera object requires a frame and two rasters. The 
 * frame is required so the camera can be positioned within world space. 
 *
 * The rasters will usually be of type rwRASTERTYPECAMERARASTER and rwRASTERTYPEZBUFFER, 
 * representing the frame buffer and Z buffer respectively. Alternatively, a camera can 
 * reference a sub-raster stored within another camera to allow split-screen or 
 * picture-in-picture rendering. 
 *
 * Finally, RenderWare Graphics allows rendering to a texture. 
 * This requires using a frame buffer raster of type rwRASTERTYPECAMERATEXTURE. 
 * In addition, rendering to a texture functionality may require platform-specific 
 * API calls - you should check any platform-specific RenderWare Graphics documentation provided 
 * for more information on this feature.
 *
 * More information can be found in the \e Cameras chapter of the User Guide.
 *
*/


/****************************************************************************
 Includes
 */

#include "batypes.h"


#include "badebug.h"
#include "balibtyp.h"

#include "batkreg.h"

#include "bacamera.h"



/****************************************************************************
 Local Defines
 */

#define RWCAMERAGLOBAL(var)                     \
   (RWPLUGINOFFSET(rwCameraGlobals,             \
    RwEngineInstance,                           \
    cameraModule.globalsOffset)->var)

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    cameraTKList =
    { sizeof(RwCamera), 
      sizeof(RwCamera), 
      0, 
      0, 
      (RwPluginRegEntry *)NULL, 
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo cameraModule;








/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwCameraClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwCameraClose(void *instance,
               RwInt32 __RWUNUSED__ offset, 
               RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwCameraClose"));

    if (RWCAMERAGLOBAL(cameraFreeList))
    {
        RwFreeListDestroy(RWCAMERAGLOBAL(cameraFreeList));
        RWCAMERAGLOBAL(cameraFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    cameraModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

/****************************************************************************
 _rwCameraOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwCameraOpen(void * instance, 
              RwInt32 offset, 
              RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwCameraOpen"));

    /* Store the data block offset (the same for all instances) */
    cameraModule.globalsOffset = offset;

    /* Create the cameras */
    RWCAMERAGLOBAL(cameraFreeList) =
        RwFreeListCreate(cameraTKList.sizeOfStruct, 4, rwCAMERAALIGNMENT);
    if (!RWCAMERAGLOBAL(cameraFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    cameraModule.numInstances++;

    /* Success */
    RWRETURN(instance);
}











